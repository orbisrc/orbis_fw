/*
 * usbdatatransfer.c
 *
 *  Created on: Apr 2, 2020
 *      Author: ivnss
 */

#include "core/iosettings.h"
#include "core/rcchannel.h"
#include "usbd_customhid.h"
#include "usb/usbdatatransfer.h"
#include "core/buffer.h"
#include <core/discrete.h>
#include "core/common.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

#define USB_LOGICAL_MINIMUM			-127
#define USB_LOGICAL_MAXIMUM			127

void USB_Handler()

{
	if(CommonSettings.USBmode == __Joystick){

		  uint8_t HID_Buffer[20] = {0};

		  HID_Buffer[0] = USB_DataScaling(RCChanelGetValue(&RCChanel[0]));	//	A
		  HID_Buffer[1] = USB_DataScaling(RCChanelGetValue(&RCChanel[1]));	//	E
		  HID_Buffer[2] = USB_DataScaling(RCChanelGetValue(&RCChanel[2]));	//	T
		  HID_Buffer[3] = USB_DataScaling(RCChanelGetValue(&RCChanel[3]));	//	R

		  /*
		   * Switches
		   */
		  HID_Buffer[4] |= (DiscreteChannelGetState(&AuxSW[SWA_IN]) == SW_H ? 1 : 0) << 0;
		  HID_Buffer[4] |= (DiscreteChannelGetState(&AuxSW[SWB_IN]) == SW_H ? 1 : 0) << 1;
		  HID_Buffer[4] |= (DiscreteChannelGetState(&AuxSW[SWC_IN]) == SW_H ? 1 : 0) << 2;
		  HID_Buffer[4] |= (DiscreteChannelGetState(&AuxSW[SWD_IN]) == SW_H ? 1 : 0) << 3;
		  HID_Buffer[4] |= (DiscreteChannelGetState(&AuxSW[SWE_IN]) == SW_H ? 1 : 0) << 4;
		  HID_Buffer[4] |= (DiscreteChannelGetState(&AuxSW[SWF_IN]) == SW_H ? 1 : 0) << 5;

		/*
		 *  ?
		 */
		  HID_Buffer[5] = 0;
		  HID_Buffer[6] = 0;
		  HID_Buffer[7] = 0;

		  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, HID_Buffer, 7);

	}

}


uint8_t USB_DataScaling(uint16_t Value)
{
	uint16_t Result;

	Result = (uint16_t)(Value * (USB_LOGICAL_MAXIMUM - USB_LOGICAL_MINIMUM)/(BASE_CH_MAX - BASE_CH_MIN) + USB_LOGICAL_MINIMUM);

	return (uint8_t)Result;
}
