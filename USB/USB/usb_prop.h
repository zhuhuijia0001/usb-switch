/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_prop.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : All processings related to UsbSwitch Mouse demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_PROP_H
#define __USB_PROP_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _HID_REQUESTS
{
  GET_REPORT = 1,
  GET_IDLE,
  GET_PROTOCOL,

  SET_REPORT = 9,
  SET_IDLE,
  SET_PROTOCOL
} HID_REQUESTS;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void UsbSwitch_init(void);
void UsbSwitch_Reset(void);
void UsbSwitch_SetConfiguration(void);
void UsbSwitch_SetDeviceAddress (void);
void UsbSwitch_Status_In (void);
void UsbSwitch_Status_Out (void);
RESULT UsbSwitch_Data_Setup(u8);
RESULT UsbSwitch_NoData_Setup(u8);
RESULT UsbSwitch_Get_Interface_Setting(u8 Interface, u8 AlternateSetting);
u8 *UsbSwitch_GetDeviceDescriptor(u16 );
u8 *UsbSwitch_GetConfigDescriptor(u16);
u8 *UsbSwitch_GetStringDescriptor(u16);
RESULT UsbSwitch_SetProtocol(void);
u8 *UsbSwitch_GetProtocolValue(u16 Length);
RESULT UsbSwitch_SetProtocol(void);
u8 *UsbSwitch_GetReportDescriptor(u16 Length);
u8 *UsbSwitch_GetHIDDescriptor(u16 Length);

/* Exported define -----------------------------------------------------------*/
#define UsbSwitch_GetConfiguration          NOP_Process
//#define UsbSwitch_SetConfiguration          NOP_Process
#define UsbSwitch_GetInterface              NOP_Process
#define UsbSwitch_SetInterface              NOP_Process
#define UsbSwitch_GetStatus                 NOP_Process
#define UsbSwitch_ClearFeature              NOP_Process
#define UsbSwitch_SetEndPointFeature        NOP_Process
#define UsbSwitch_SetDeviceFeature          NOP_Process
//#define UsbSwitch_SetDeviceAddress          NOP_Process

#define REPORT_DESCRIPTOR                  0x22

#endif /* __USB_PROP_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
