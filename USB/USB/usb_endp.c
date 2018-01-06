/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_istr.h"

#include "rtthread.h"

#include "Protocol.h"
#include "Packet.h"
#include "Task.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    : EP1 IN Callback Routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
//for data sent to host
void EP1_IN_Callback(void)
{
	
}

static void SendEP1Data(u8 *data, u16 len)
{
	UserToPMABufferCopy(data, ENDP1_TXADDR, len);
	SetEPTxCount(ENDP1, len);
	SetEPTxValid(ENDP1);
}

/*******************************************************************************
* Function Name  : EP1_OUT_Callback.
* Description    : EP1 OUT Callback Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
//for data received from host
void EP1_OUT_Callback(void)
{
	u8 DataLen;
	u8 buffer[16];
	
	DataLen = GetEPRxCount(ENDP1); 
	PMAToUserBufferCopy(buffer, ENDP1_RXADDR, DataLen); 

	u8 cmd = GetPacketCmd(buffer);
	action_msg msg;
	msg.cmd = cmd;
	msg.action = ACTION_START;

	bool validCmd = TRUE;
	
	switch (cmd)
	{
	case CMD_SWITCH_MAINBOARD:
		{
			int port;
			ParseSwitchMainboardPacket(buffer, &port);
			if (port >= 0 && port < MAINBOARD_PORT_NUM)
			{
				msg.content.port = port;
			}
			else
			{
				validCmd = FALSE;

				u8 len = BuildSwitchMainboardRetPacket(buffer, sizeof(buffer), RESULT_FAILED, port);
				SendEP1Data(buffer, len);
			}
		}
		
		break;

	case CMD_SWITCH_SLAVEBOARD:
		{
			int mainPort, slavePort;
			ParseSWitchSlaveboardPacket(buffer, &mainPort, &slavePort);
			if (mainPort >= 0 && mainPort < MAINBOARD_PORT_NUM
				&& slavePort >= 0 && slavePort < SLAVEBOARD_PORT_NUM)
			{
				msg.content.switch_slave_port.main_port = mainPort;
				msg.content.switch_slave_port.slave_port = slavePort;
			}
			else
			{
				validCmd = FALSE;

				u8 len = BuildSwitchSlaveboardRetPacket(buffer, sizeof(buffer), RESULT_FAILED, 
																		mainPort,
																		slavePort);
				SendEP1Data(buffer, len);
			}
		}
		
		break;

	case CMD_DISABLE_MAINBOARD:

		break;

	default:
		break;
	}

	if (validCmd)
	{
		notify_action_msg(&msg);
	}

	SetEPRxValid(ENDP1);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

