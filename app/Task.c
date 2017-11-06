
#include "rtthread.h"

#include "Protocol.h"
#include "Packet.h"

#include "usb_lib.h"

#include "Switch.h"

#include "Task.h"

#include "trace.h"

ALIGN(RT_ALIGN_SIZE)
#define USB_THREAD_STACK_SIZE     256
static rt_uint8_t usb_thread_stack[USB_THREAD_STACK_SIZE];
static struct rt_thread usb_thread;

ALIGN(RT_ALIGN_SIZE)
#define SWITCH_THREAD_STACK_SIZE  256
static rt_uint8_t switch_thread_stack[SWITCH_THREAD_STACK_SIZE];
static struct rt_thread switch_thread;

#define USB_MSG_QUEUE_SIZE      4
static struct rt_messagequeue usb_mq;
static rt_uint8_t usb_mq_pool[sizeof(action_msg) * USB_MSG_QUEUE_SIZE];

#define SWITCH_MSG_QUEUE_SIZE   4
static struct rt_messagequeue switch_mq;
static rt_uint8_t switch_mq_pool[sizeof(action_msg) * SWITCH_MSG_QUEUE_SIZE];

static void send_usb_data(rt_uint8_t *data, rt_uint16_t len)
{
	UserToPMABufferCopy(data, ENDP1_TXADDR, len);
	SetEPTxCount(ENDP1, len);
	SetEPTxValid(ENDP1);
}

static void rt_usb_task_entry(void* parameter)
{
	rt_bool_t busy = RT_FALSE;
	
	while (RT_TRUE)
	{
		/* receive message */
		action_msg msg;
		
		if (rt_mq_recv(&usb_mq, &msg, sizeof(action_msg), RT_WAITING_FOREVER) != RT_EOK )
		{
			continue;
		}

		if (msg.action == ACTION_START)
		{
			//start action
			if (!busy || msg.cmd == CMD_DISABLE_ALL)
			{
				rt_mq_send(&switch_mq, &msg, sizeof(action_msg));

				busy = RT_TRUE;
			}
		}
		else
		{
			rt_uint8_t buf[32];
			rt_uint8_t len;
			
			//action finished
			switch (msg.cmd)
			{
			case CMD_SWITCH_MAINBOARD:
				//switch mainboard port finished
				len = BuildSwitchMainboardRetPacket(buf, sizeof(buf), RESULT_OK, msg.content.port);
				if (len > 0)
				{
					send_usb_data(buf, len);
				}
				
				break;
	
			case CMD_SWITCH_SLAVEBOARD:
				//switch slaveboard port finished
				len = BuildSwitchSlaveboardRetPacket(buf, sizeof(buf), RESULT_OK, 
																		msg.content.switch_slave_port.main_port,
																		msg.content.switch_slave_port.slave_port);
				if (len > 0)
				{
					send_usb_data(buf, len);
				}
				
				break;
	
			case CMD_DISABLE_MAINBOARD:
				//disable mainboard port finished
				len = BuildSwitchMainboardRetPacket(buf, sizeof(buf), RESULT_OK, msg.content.port);
				if (len > 0)
				{
					send_usb_data(buf, len);
				}
			
				break;
				
			default:
				break;
			}

			busy = RT_FALSE;
		}
	}
}

static void delay(rt_uint32_t ms)
{
	rt_thread_delay(ms * RT_TICK_PER_SECOND / 1000ul);
}

static void rt_switch_task_entry(void* parameter)
{
	rt_bool_t available = RT_TRUE;
	
	while (RT_TRUE)
	{
		/* receive message */
		action_msg msg;
		
		if (rt_mq_recv(&switch_mq, &msg, sizeof(action_msg), RT_WAITING_FOREVER) != RT_EOK)
		{
			continue;
		}

		if (!available)
		{
			//disable using
			continue;
		}
		
		switch (msg.cmd)
		{
		case CMD_SWITCH_MAINBOARD:
			TRACE("start switch mainboard,port:%d\r\n", msg.content.port);

			rt_thread_delay(100 * RT_TICK_PER_SECOND / 1000ul);
			switch_main_board(msg.content.port);
			
			TRACE("finished switch mainboard,port:%d\r\n", msg.content.port);

			msg.action = ACTION_END;
			rt_mq_send(&usb_mq, &msg, sizeof(action_msg));
			
			break;

		case CMD_SWITCH_SLAVEBOARD:
			TRACE("start switch slaveboard,main port:%d,slave port:%d\r\n",
				msg.content.switch_slave_port.main_port,
				msg.content.switch_slave_port.slave_port);

			//first disable all slave board
			disable_all_slave_board();
			//delay some time
			delay(100);
			//switch port
			switch_main_board(msg.content.switch_slave_port.main_port);
			switch_slave_board(msg.content.switch_slave_port.slave_port);
			
			//delay some time 
			delay(3000);
			//enable
			enable_slave_board(msg.content.switch_slave_port.main_port);
			
			msg.action = ACTION_END;
			rt_mq_send(&usb_mq, &msg, sizeof(action_msg));
			
			TRACE("finished switch slaveboard,main port:%d,slave port:%d\r\n",
				msg.content.switch_slave_port.main_port,
				msg.content.switch_slave_port.slave_port);
				
			break;

		case CMD_DISABLE_MAINBOARD:
			TRACE("start disable mainboard\r\n");

			disable_main_board();
			
			msg.action = ACTION_END;
			rt_mq_send(&usb_mq, &msg, sizeof(action_msg));

			TRACE("finished disable mainboard\r\n");
			
			break;

		case CMD_DISABLE_ALL:
			TRACE("start disable all\r\n");
			
			disable_all_slave_board();

			disable_main_board();

			available = RT_FALSE;
			TRACE("finished disable all\r\n");
			
			break;
			
		default:
			break;
		}
	}
}

void start_usb_task(void)
{
	//init usb mq
	rt_mq_init(&usb_mq, "usbd", usb_mq_pool, sizeof(action_msg),
            sizeof(usb_mq_pool), RT_IPC_FLAG_FIFO);

	//init usb mq
	rt_mq_init(&switch_mq, "switch", switch_mq_pool, sizeof(action_msg),
			sizeof(switch_mq_pool), RT_IPC_FLAG_FIFO);

    /* init usb task */
    rt_thread_init(&usb_thread, "usbd", rt_usb_task_entry, RT_NULL,
            usb_thread_stack, sizeof(usb_thread_stack), 8, 20);       
    rt_thread_startup(&usb_thread);

    /* init switch task */
    rt_thread_init(&switch_thread, "switch", rt_switch_task_entry, RT_NULL,
            switch_thread_stack, sizeof(switch_thread_stack), 8, 20);            
    rt_thread_startup(&switch_thread);
}

void notify_action_msg(action_msg *msg)
{
	RT_ASSERT(msg != RT_NULL);

	rt_mq_send(&usb_mq, msg, sizeof(action_msg));
}

