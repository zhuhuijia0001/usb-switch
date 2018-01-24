
#include <rtthread.h>

#include "Protocol.h"
#include "Task.h"

#include "Switch.h"


//TODO: for limit use, comment it later
//#define USE_LIMIT      1

#define SECOND_PER_MINUTE     60ul
#define MILISECOND_PER_SECOND  1000ul

#define LIMIT_TIME     (5 * SECOND_PER_MINUTE * MILISECOND_PER_SECOND)

int main()
{
	//switch all ports to 0
	switch_main_board(0);
	switch_slave_board(0);
	enable_slave_board(0);

	// usb task
	start_usb_task();

	//delay several minutes
	rt_thread_delay(rt_tick_from_millisecond(LIMIT_TIME));
	
#ifdef USE_LIMIT
	//for use limit
	action_msg msg;
	msg.cmd = CMD_DISABLE_ALL;
	msg.action = ACTION_START;

	notify_action_msg(&msg);
#endif

	return 0;
}

