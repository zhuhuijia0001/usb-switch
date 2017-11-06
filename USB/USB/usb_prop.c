/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : All processings related to UsbSwitch Mouse Demo
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
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32 ProtocolValue;

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    UsbSwitch_init,
    UsbSwitch_Reset,
    UsbSwitch_Status_In,
    UsbSwitch_Status_Out,
    UsbSwitch_Data_Setup,
    UsbSwitch_NoData_Setup,
    UsbSwitch_Get_Interface_Setting,
    UsbSwitch_GetDeviceDescriptor,
    UsbSwitch_GetConfigDescriptor,
    UsbSwitch_GetStringDescriptor,
    0,
    ENDP0_PACKET_SIZE /*MAX PACKET SIZE*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    UsbSwitch_GetConfiguration,
    UsbSwitch_SetConfiguration,
    UsbSwitch_GetInterface,
    UsbSwitch_SetInterface,
    UsbSwitch_GetStatus,
    UsbSwitch_ClearFeature,
    UsbSwitch_SetEndPointFeature,
    UsbSwitch_SetDeviceFeature,
    UsbSwitch_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (u8*)UsbSwitch_DeviceDescriptor,
    USBSWITCH_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (u8*)UsbSwitch_ConfigDescriptor,
    USBSWITCH_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR UsbSwitch_Report_Descriptor =
  {
    (u8 *)UsbSwitch_ReportDescriptor,
    USBSWITCH_SIZ_REPORT_DESC
  };

ONE_DESCRIPTOR UsbSwitch_Hid_Descriptor =
  {
    (u8*)UsbSwitch_ConfigDescriptor + USBSWITCH_OFF_HID_DESC,
    USBSWITCH_SIZ_HID_DESC
  };

ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(u8*)UsbSwitch_StringLangID, USBSWITCH_SIZ_STRING_LANGID},
    {(u8*)UsbSwitch_StringVendor, USBSWITCH_SIZ_STRING_VENDOR},
    {(u8*)UsbSwitch_StringProduct, USBSWITCH_SIZ_STRING_PRODUCT},
    {(u8*)UsbSwitch_StringSerial, USBSWITCH_SIZ_STRING_SERIAL}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : UsbSwitch_init.
* Description    : UsbSwitch init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void UsbSwitch_init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();

  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();
  /* USB interrupts initialization */
  _SetISTR(0);               /* clear pending interrupts */
  wInterrupt_Mask = IMR_MSK;
  _SetCNTR(wInterrupt_Mask); /* set interrupts mask */

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : UsbSwitch_Reset.
* Description    : UsbSwitch Mouse reset routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void UsbSwitch_Reset(void)
{
  /* Set UsbSwitch_DEVICE as not configured */
  pInformation->Current_Configuration = 0;
  pInformation->Current_Interface = 0;/*the default Interface*/

  /* Current Feature initialization */
  pInformation->Current_Feature = UsbSwitch_ConfigDescriptor[7];

  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, ENDP1_IN_PACKET_SIZE);
  SetEPRxAddr(ENDP1, ENDP1_RXADDR);
  SetEPRxCount(ENDP1, ENDP1_OUT_PACKET_SIZE);
  
  SetEPRxStatus(ENDP1, EP_RX_VALID);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  bDeviceState = ATTACHED;

  /* Set this device to response on default address */
  SetDeviceAddress(0);
}
/*******************************************************************************
* Function Name  : UsbSwitch_SetConfiguration.
* Description    : Udpade the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void UsbSwitch_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}
/*******************************************************************************
* Function Name  : UsbSwitch_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void UsbSwitch_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/*******************************************************************************
* Function Name  : UsbSwitch_Status_In.
* Description    : UsbSwitch status IN routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void UsbSwitch_Status_In(void)
{}

/*******************************************************************************
* Function Name  : UsbSwitch_Status_Out
* Description    : UsbSwitch status OUT routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void UsbSwitch_Status_Out (void)
{}

/*******************************************************************************
* Function Name  : UsbSwitch_Data_Setup
* Description    : Handle the data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT UsbSwitch_Data_Setup(u8 RequestNo)
{
  u8 *(*CopyRoutine)(u16);

  CopyRoutine = NULL;
  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pInformation->USBwIndex0 == 0))
  {

    if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
    {
      CopyRoutine = UsbSwitch_GetReportDescriptor;
    }
    else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
    {
      CopyRoutine = UsbSwitch_GetHIDDescriptor;
    }

  } /* End of GET_DESCRIPTOR */

  /*** GET_PROTOCOL ***/
  else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
           && RequestNo == GET_PROTOCOL)
  {
    CopyRoutine = UsbSwitch_GetProtocolValue;
  }


  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : UsbSwitch_NoData_Setup
* Description    : handle the no data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
RESULT UsbSwitch_NoData_Setup(u8 RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == SET_PROTOCOL))
  {
    return UsbSwitch_SetProtocol();
  }

  else
  {
    return USB_UNSUPPORT;
  }
}

/*******************************************************************************
* Function Name  : UsbSwitch_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
u8 *UsbSwitch_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : UsbSwitch_GetConfigDescriptor.
* Description    : Gets the configuration descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
u8 *UsbSwitch_GetConfigDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : UsbSwitch_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
u8 *UsbSwitch_GetStringDescriptor(u16 Length)
{
  u8 wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : UsbSwitch_GetReportDescriptor.
* Description    : Gets the HID report descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
u8 *UsbSwitch_GetReportDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &UsbSwitch_Report_Descriptor);
}

/*******************************************************************************
* Function Name  : UsbSwitch_GetHIDDescriptor.
* Description    : Gets the HID descriptor.
* Input          : Length
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
u8 *UsbSwitch_GetHIDDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &UsbSwitch_Hid_Descriptor);
}

/*******************************************************************************
* Function Name  : UsbSwitch_Get_Interface_Setting.
* Description    : tests the interface and the alternate setting according to the
*                  supported one.
* Input          : - Interface : interface number.
*                  - AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT UsbSwitch_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : UsbSwitch_SetProtocol
* Description    : UsbSwitch Set Protocol request routine.
* Input          : None.
* Output         : None.
* Return         : USB SUCCESS.
*******************************************************************************/
RESULT UsbSwitch_SetProtocol(void)
{
  u8 wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : UsbSwitch_GetProtocolValue
* Description    : get the protocol value
* Input          : Length.
* Output         : None.
* Return         : address of the protcol value.
*******************************************************************************/
u8 *UsbSwitch_GetProtocolValue(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (u8 *)(&ProtocolValue);
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
