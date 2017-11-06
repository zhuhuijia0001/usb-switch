
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

//cmd type
#define CMD_SWITCH_MAINBOARD        0x01
#define CMD_SWITCH_SLAVEBOARD       0x02
#define CMD_DISABLE_MAINBOARD       0x03

#define CMD_DISABLE_ALL             0x04 //stop using

//result
#define RESULT_OK       0
#define RESULT_FAILED   1

#define MAINBOARD_PORT_NUM   16
#define SLAVEBOARD_PORT_NUM  8

#endif
