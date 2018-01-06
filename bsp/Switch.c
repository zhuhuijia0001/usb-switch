
#include "rtthread.h"

#include "PinDefine.h"
#include "Protocol.h"
#include "Switch.h"

#include "trace.h"

int init_switch(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

	/* pin for main board 1/2 data switch */
	GPIO_InitStructure.GPIO_Pin = GET_GPIO_PIN(PIN_USB_A_SEL);			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GET_GPIO_TYPE(PIN_USB_A_SEL), &GPIO_InitStructure);

	CLR_GPIO_BIT(PIN_USB_A_SEL);

	/* pin for main board 1/2 enable switch */
	GPIO_InitStructure.GPIO_Pin = GET_GPIO_PIN(PIN_EN_A) | GET_GPIO_PIN(PIN_EN_B);			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GET_GPIO_TYPE(PIN_EN_A), &GPIO_InitStructure);

	CLR_GPIO_BIT(PIN_EN_A);
	CLR_GPIO_BIT(PIN_EN_B);
	
	/* 74238, for main board 1/8 enable switch */
	GPIO_InitStructure.GPIO_Pin = GET_GPIO_PIN(PIN_238_A) | GET_GPIO_PIN(PIN_238_B);			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GET_GPIO_TYPE(PIN_238_A), &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GET_GPIO_PIN(PIN_238_C) | GET_GPIO_PIN(PIN_238_SEL) | GET_GPIO_PIN(PIN_238_EN);			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GET_GPIO_TYPE(PIN_238_C), &GPIO_InitStructure);

	SET_GPIO_BIT(PIN_238_EN);
	
	/* pin for main board 1/8 data switch */
	GPIO_InitStructure.GPIO_Pin = GET_GPIO_PIN(PIN_C0) | GET_GPIO_PIN(PIN_C1) | GET_GPIO_PIN(PIN_C2);			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GET_GPIO_TYPE(PIN_C0), &GPIO_InitStructure);

	CLR_GPIO_BIT(PIN_C0);
	CLR_GPIO_BIT(PIN_C1);
	CLR_GPIO_BIT(PIN_C2);
	
	/* pin for slave board 1/8 switch */
	GPIO_InitStructure.GPIO_Pin = GET_GPIO_PIN(PIN_SLAVE_C0) | GET_GPIO_PIN(PIN_SLAVE_C1) | GET_GPIO_PIN(PIN_SLAVE_C2);			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GET_GPIO_TYPE(PIN_SLAVE_C0), &GPIO_InitStructure);

	return RT_EOK;
}
INIT_BOARD_EXPORT(init_switch);

static void switch_main_port_0()
{
	CLR_GPIO_BIT(PIN_C0);
	CLR_GPIO_BIT(PIN_C1);
	CLR_GPIO_BIT(PIN_C2);
}

static void switch_main_port_1()
{
	SET_GPIO_BIT(PIN_C0);
	CLR_GPIO_BIT(PIN_C1);
	CLR_GPIO_BIT(PIN_C2);
}

static void switch_main_port_2()
{
	CLR_GPIO_BIT(PIN_C0);
	SET_GPIO_BIT(PIN_C1);
	CLR_GPIO_BIT(PIN_C2);
}

static void switch_main_port_3()
{
	SET_GPIO_BIT(PIN_C0);
	SET_GPIO_BIT(PIN_C1);
	CLR_GPIO_BIT(PIN_C2);
}

static void switch_main_port_4()
{
	CLR_GPIO_BIT(PIN_C0);
	CLR_GPIO_BIT(PIN_C1);
	SET_GPIO_BIT(PIN_C2);
}

static void switch_main_port_5()
{
	SET_GPIO_BIT(PIN_C0);
	CLR_GPIO_BIT(PIN_C1);
	SET_GPIO_BIT(PIN_C2);
}

static void switch_main_port_6()
{
	CLR_GPIO_BIT(PIN_C0);
	SET_GPIO_BIT(PIN_C1);
	SET_GPIO_BIT(PIN_C2);
}

static void switch_main_port_7()
{
	SET_GPIO_BIT(PIN_C0);
	SET_GPIO_BIT(PIN_C1);
	SET_GPIO_BIT(PIN_C2);
}

static void (*const s_switch_main_port_fun[])() = 
{
	[0] = switch_main_port_0,
	[1] = switch_main_port_1,
	[2] = switch_main_port_2,
	[3] = switch_main_port_3,
	[4] = switch_main_port_4,
	[5] = switch_main_port_5,
	[6] = switch_main_port_6,
	[7] = switch_main_port_7,
};

rt_bool_t switch_main_board(int port)
{
	if (port < 0 || port >= MAINBOARD_PORT_NUM)
	{
		return RT_FALSE;
	}

	TRACE("switch mainboard:%d\r\n", port);
	
	s_switch_main_port_fun[port & 0x07]();
	if (port < 8)
	{
		//data switch
		CLR_GPIO_BIT(PIN_USB_A_SEL);
		
		SET_GPIO_BIT(PIN_EN_A);
		CLR_GPIO_BIT(PIN_EN_B);
	}
	else
	{
		//data switch
		SET_GPIO_BIT(PIN_USB_A_SEL);
		
		CLR_GPIO_BIT(PIN_EN_A);
		SET_GPIO_BIT(PIN_EN_B);
	}
	
	return RT_TRUE;
}

void disable_main_board(void)
{
	CLR_GPIO_BIT(PIN_EN_A);

	CLR_GPIO_BIT(PIN_EN_B);
}

void disable_all_slave_board(void)
{
	//disable
	SET_GPIO_BIT(PIN_238_EN);
}

static void enable_slaveboard_0()
{
	CLR_GPIO_BIT(PIN_238_A);
	CLR_GPIO_BIT(PIN_238_B);
	CLR_GPIO_BIT(PIN_238_C);
}

static void enable_slaveboard_1()
{
	SET_GPIO_BIT(PIN_238_A);
	CLR_GPIO_BIT(PIN_238_B);
	CLR_GPIO_BIT(PIN_238_C);	
}

static void enable_slaveboard_2()
{
	CLR_GPIO_BIT(PIN_238_A);
	SET_GPIO_BIT(PIN_238_B);
	CLR_GPIO_BIT(PIN_238_C);	
}

static void enable_slaveboard_3()
{
	SET_GPIO_BIT(PIN_238_A);
	SET_GPIO_BIT(PIN_238_B);
	CLR_GPIO_BIT(PIN_238_C);	
}

static void enable_slaveboard_4()
{
	CLR_GPIO_BIT(PIN_238_A);
	CLR_GPIO_BIT(PIN_238_B);
	SET_GPIO_BIT(PIN_238_C);	
}

static void enable_slaveboard_5()
{
	SET_GPIO_BIT(PIN_238_A);
	CLR_GPIO_BIT(PIN_238_B);
	SET_GPIO_BIT(PIN_238_C);	
}

static void enable_slaveboard_6()
{
	CLR_GPIO_BIT(PIN_238_A);
	SET_GPIO_BIT(PIN_238_B);
	SET_GPIO_BIT(PIN_238_C);	
}

static void enable_slaveboard_7()
{
	SET_GPIO_BIT(PIN_238_A);
	SET_GPIO_BIT(PIN_238_B);
	SET_GPIO_BIT(PIN_238_C);	
}

static void (*const s_enable_slaveboard_fun[])() = 
{
	[0] = enable_slaveboard_0,
	[1] = enable_slaveboard_1,
	[2] = enable_slaveboard_2,
	[3] = enable_slaveboard_3,
	[4] = enable_slaveboard_4,
	[5] = enable_slaveboard_5,
	[6] = enable_slaveboard_6,
	[7] = enable_slaveboard_7,
};

rt_bool_t enable_slave_board(int main_port)
{
	if (main_port < 0 || main_port >= MAINBOARD_PORT_NUM)
	{
		return RT_FALSE;
	}

	TRACE("enable slaveboard:%d\r\n", main_port);
	
	s_enable_slaveboard_fun[main_port & 0x07]();
		
	if (main_port < 8)
	{
		SET_GPIO_BIT(PIN_238_SEL);
	}
	else
	{
		CLR_GPIO_BIT(PIN_238_SEL);
	}
	
	//enable
	CLR_GPIO_BIT(PIN_238_EN);
	
	return RT_TRUE;
}

static void switch_slave_port_0()
{
	CLR_GPIO_BIT(PIN_SLAVE_C0);
	CLR_GPIO_BIT(PIN_SLAVE_C1);
	CLR_GPIO_BIT(PIN_SLAVE_C2);
}

static void switch_slave_port_1()
{
	SET_GPIO_BIT(PIN_SLAVE_C0);
	CLR_GPIO_BIT(PIN_SLAVE_C1);
	CLR_GPIO_BIT(PIN_SLAVE_C2);
}

static void switch_slave_port_2()
{
	CLR_GPIO_BIT(PIN_SLAVE_C0);
	SET_GPIO_BIT(PIN_SLAVE_C1);
	CLR_GPIO_BIT(PIN_SLAVE_C2);
}

static void switch_slave_port_3()
{
	SET_GPIO_BIT(PIN_SLAVE_C0);
	SET_GPIO_BIT(PIN_SLAVE_C1);
	CLR_GPIO_BIT(PIN_SLAVE_C2);
}

static void switch_slave_port_4()
{
	CLR_GPIO_BIT(PIN_SLAVE_C0);
	CLR_GPIO_BIT(PIN_SLAVE_C1);
	SET_GPIO_BIT(PIN_SLAVE_C2);
}

static void switch_slave_port_5()
{
	SET_GPIO_BIT(PIN_SLAVE_C0);
	CLR_GPIO_BIT(PIN_SLAVE_C1);
	SET_GPIO_BIT(PIN_SLAVE_C2);
}

static void switch_slave_port_6()
{
	CLR_GPIO_BIT(PIN_SLAVE_C0);
	SET_GPIO_BIT(PIN_SLAVE_C1);
	SET_GPIO_BIT(PIN_SLAVE_C2);
}

static void switch_slave_port_7()
{
	SET_GPIO_BIT(PIN_SLAVE_C0);
	SET_GPIO_BIT(PIN_SLAVE_C1);
	SET_GPIO_BIT(PIN_SLAVE_C2);
}

static void (*const s_switch_slave_port_fun[])() = 
{
	[0] = switch_slave_port_0,
	[1] = switch_slave_port_1,
	[2] = switch_slave_port_2,
	[3] = switch_slave_port_3,
	[4] = switch_slave_port_4,
	[5] = switch_slave_port_5,
	[6] = switch_slave_port_6,
	[7] = switch_slave_port_7,
};

rt_bool_t switch_slave_board(int slave_port)
{
	if (slave_port < 0 || slave_port >= SLAVEBOARD_PORT_NUM)
	{
		return RT_FALSE;
	}

	TRACE("switch slaveboard:%d\r\n", slave_port);
	s_switch_slave_port_fun[slave_port & 0x07]();

	return RT_TRUE;
}

