
#ifndef _TASK_H_
#define _TASK_H_

#define ACTION_START      1
#define ACTION_END        0

typedef struct
{
	int main_port;

	int slave_port;
} slave_port_struct;

typedef struct
{
	rt_uint8_t cmd;

	rt_uint8_t action;

	union
	{
		int port; //switch mainboard port

		slave_port_struct switch_slave_port; 
	} content;
} action_msg;

extern void start_usb_task(void);

extern void notify_action_msg(action_msg *msg);

#endif

