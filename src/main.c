/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "periphery/adc.h"
#include "periphery/dac.h"
#include "periphery/dma.h"
#include "fatfs.h"
#include "periphery/rtc.h"
#include "periphery/sdio.h"
#include "periphery/spi.h"
#include "periphery/tim.h"
#include "periphery/usart.h"
#include "usb_device.h"
#include "periphery/gpio.h"
#include "core/common.h"
#include "core/iosettings.h"
#include "gui_lvgl/lv_port_disp.h"
#include "gui/stdispdriver.h"
#include "lvgl.h"

void SystemClock_Config(void);

int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM10_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();

  CommonInit();

  // MX_DAC_Init();
  // /*
  //  * Wait before sd card init. 250ms min
  //  */
  // HAL_Delay(1000);
  // MX_SDIO_SD_Init();
  // MX_USB_DEVICE_Init();
  STLCDinit();

  lv_init();

  lv_port_disp_init();
  

  lv_obj_t *win = lv_win_create(lv_scr_act(), 40);
  lv_obj_t *btn;
  btn = lv_win_add_btn(win, LV_SYMBOL_LEFT, 40);

  lv_win_add_title(win, "A title");

  btn = lv_win_add_btn(win, LV_SYMBOL_RIGHT, 40);

  btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 60);

  lv_obj_t *cont = lv_win_get_content(win); /*Content can be aded here*/
  lv_obj_t *label = lv_label_create(cont);
  lv_label_set_text(label, "Lorem ipsum dolor sit amet,\n"
                            "consectetur adipiscing elit,\n"
                            "sed do eiusmod tempor\n"
                            "incididunt ut labore et\n"
                            "dolore magna aliqua. Ut\n" 
                            "enim ad minim veniam, quis\n"
                            "nostrud exercitation ullamco\n"
                            "laboris nisi ut aliquip ex ea\n"
                            "commodo consequat. Duis aute\n"
                            "irure dolor in reprehenderit in\n"
                            "voluptate velit esse cillum\n"
                            "dolore eu fugiat nulla pariatur.\n"
                            "Excepteur sint occaecat cupidatat\n"
                            "non proident, sunt in culpa qui\n"
                            "officia deserunt mollit anim id\n"
                            "est laborum. ");


  while (1)
  {
    CommonRun();
    lv_task_handler();
  
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  /*
	 *
	 *
	 *
	 *
	 */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
