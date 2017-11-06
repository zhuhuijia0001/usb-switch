/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Descriptor Header for UsbSwitch Mouse Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE                     0x21
#define USBSWITCH_SIZ_HID_DESC                   0x09
#define USBSWITCH_OFF_HID_DESC                   0x12

#define USBSWITCH_SIZ_DEVICE_DESC                18
#define USBSWITCH_SIZ_CONFIG_DESC                41
#define USBSWITCH_SIZ_REPORT_DESC                27
#define USBSWITCH_SIZ_STRING_LANGID              4
#define USBSWITCH_SIZ_STRING_VENDOR              38
#define USBSWITCH_SIZ_STRING_PRODUCT             28
#define USBSWITCH_SIZ_STRING_SERIAL              50

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

/* Exported functions ------------------------------------------------------- */
extern const u8 UsbSwitch_DeviceDescriptor[USBSWITCH_SIZ_DEVICE_DESC];
extern const u8 UsbSwitch_ConfigDescriptor[USBSWITCH_SIZ_CONFIG_DESC];
extern const u8 UsbSwitch_ReportDescriptor[USBSWITCH_SIZ_REPORT_DESC];
extern const u8 UsbSwitch_StringLangID[USBSWITCH_SIZ_STRING_LANGID];
extern const u8 UsbSwitch_StringVendor[USBSWITCH_SIZ_STRING_VENDOR];
extern const u8 UsbSwitch_StringProduct[USBSWITCH_SIZ_STRING_PRODUCT];
extern u8 UsbSwitch_StringSerial[USBSWITCH_SIZ_STRING_SERIAL];

#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
