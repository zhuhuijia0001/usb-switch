/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2013-07-12     aozima       update for auto initial.
 */

/**
 * @addtogroup STM32
 */
/*@{*/

#include <board.h>
#include <rtthread.h>

#ifdef  RT_USING_COMPONENTS_INIT
#include <components.h>
#endif  /* RT_USING_COMPONENTS_INIT */

#include "Protocol.h"
#include "Task.h"

#include "Switch.h"

#include "trace.h"

//for limit use
#define USE_LIMIT      1

static void rt_init_thread_entry(void* parameter)
{
#ifdef RT_USING_COMPONENTS_INIT
    /* initialization RT-Thread Components */
    rt_components_init();
#endif

	switch_main_board(0);
	switch_slave_board(0);
	enable_slave_board(0);

	// usb task
	start_usb_task();

	//delay several minutes
	rt_thread_delay(3 * 60 * 1000 * RT_TICK_PER_SECOND / 1000ul);
	
#ifdef USE_LIMIT
	//for use limit
	action_msg msg;
	msg.cmd = CMD_DISABLE_ALL;
	msg.action = ACTION_START;

	notify_action_msg(&msg);
#endif

}

int rt_application_init(void)
{
	rt_thread_t thread;

	thread = rt_thread_create("init", rt_init_thread_entry, RT_NULL, 1024, 8, 20);

    if (thread != RT_NULL)
        rt_thread_startup(thread);

    return 0;
}

/*@}*/

