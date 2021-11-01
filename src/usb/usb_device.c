/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usb_device.c
  * @version        : v1.0_Cube
  * @brief          : This file implements the USB Device
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_customhid.h"
#include "usbd_custom_hid_if.h"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "usbd_msc.h"
#include "usbd_storage_if.h"
#include "core/iosettings.h"
#include "core/common.h"

USBD_HandleTypeDef hUsbDeviceFS;

void MX_USB_DFU_Init(void)
{

}

void MX_USB_CDC_Init(void)
{
	  if (USBD_Init(&hUsbDeviceFS, &FS_CDC_Desc, DEVICE_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
	  {
	    Error_Handler();
	  }
}

void MX_USB_CustomHID_Init(void)
{
	  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CUSTOM_HID) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_CUSTOM_HID_RegisterInterface(&hUsbDeviceFS, &USBD_CustomHID_fops_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
	  {
	    Error_Handler();
	  }
}

void MX_USB_MS_Init(void)
{
	  if (USBD_Init(&hUsbDeviceFS, &FS_MS_Desc, DEVICE_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_MSC) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_MSC_RegisterStorage(&hUsbDeviceFS, &USBD_Storage_Interface_fops_FS) != USBD_OK)
	  {
	    Error_Handler();
	  }
	  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
	  {
	    Error_Handler();
	  }
}

void MX_USB_DEVICE_Init(void)
{

	switch (CommonSettings.USBmode) {

		case __VCOM:

			MX_USB_CDC_Init();

			break;

		case __Joystick:

			MX_USB_CustomHID_Init();

			break;

		case __SDcardReader:

			MX_USB_MS_Init();

			break;


		default:
			break;
	}
	
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
