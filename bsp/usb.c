#include "rtthread.h"

#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"

#include "usb.h"

int init_usb(void)
{
	/* Initilize Usb */
	USB_Interrupts_Config();    
	Set_USBClock();   
	USB_Init();

	return RT_EOK;
}
INIT_BOARD_EXPORT(init_usb);

