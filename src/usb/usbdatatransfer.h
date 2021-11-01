/*
 * usbdatatransfer.h
 *
 *  Created on: Apr 2, 2020
 *      Author: ivnss
 */

#ifndef INC_STUSB_USBDATATRANSFER_H_
#define INC_STUSB_USBDATATRANSFER_H_

#include "usbd_customhid.h"


void USB_Handler();

uint8_t USB_DataScaling(uint16_t Value);

#endif /* INC_STUSB_USBDATATRANSFER_H_ */
