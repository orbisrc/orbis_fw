/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BEEPER_Pin GPIO_PIN_2
#define BEEPER_GPIO_Port GPIOE
#define VIBRO_Pin GPIO_PIN_3
#define VIBRO_GPIO_Port GPIOE
#define SPI1_DC_Pin GPIO_PIN_5
#define SPI1_DC_GPIO_Port GPIOC
#define SPI1_RST_Pin GPIO_PIN_0
#define SPI1_RST_GPIO_Port GPIOB
#define SWD_H_Pin GPIO_PIN_7
#define SWD_H_GPIO_Port GPIOE
#define BLK_Pin GPIO_PIN_9
#define BLK_GPIO_Port GPIOE
#define SWB_H_Pin GPIO_PIN_13
#define SWB_H_GPIO_Port GPIOE
#define SWB_L_Pin GPIO_PIN_14
#define SWB_L_GPIO_Port GPIOE
#define SWA_H_Pin GPIO_PIN_15
#define SWA_H_GPIO_Port GPIOE
#define SWA_L_Pin GPIO_PIN_10
#define SWA_L_GPIO_Port GPIOB
#define SPI2_CS_Pin GPIO_PIN_12
#define SPI2_CS_GPIO_Port GPIOB
#define SPI2_DC_Pin GPIO_PIN_9
#define SPI2_DC_GPIO_Port GPIOD
#define SWF_H_Pin GPIO_PIN_13
#define SWF_H_GPIO_Port GPIOD
#define SWF_L_Pin GPIO_PIN_14
#define SWF_L_GPIO_Port GPIOD
#define BPR_Pin GPIO_PIN_15
#define BPR_GPIO_Port GPIOD
#define SPI1_CS_Pin GPIO_PIN_15
#define SPI1_CS_GPIO_Port GPIOA
#define SWE_L_Pin GPIO_PIN_0
#define SWE_L_GPIO_Port GPIOD
#define SWE_H_Pin GPIO_PIN_1
#define SWE_H_GPIO_Port GPIOD
#define SWD_L_Pin GPIO_PIN_4
#define SWD_L_GPIO_Port GPIOD
#define SWD_HD5_Pin GPIO_PIN_5
#define SWD_HD5_GPIO_Port GPIOD
#define SWC_L_Pin GPIO_PIN_6
#define SWC_L_GPIO_Port GPIOD
#define SWC_H_Pin GPIO_PIN_7
#define SWC_H_GPIO_Port GPIOD
#define EN_B_Pin GPIO_PIN_6
#define EN_B_GPIO_Port GPIOB
#define EN_A_Pin GPIO_PIN_7
#define EN_A_GPIO_Port GPIOB
#define BTN_ENTR_Pin GPIO_PIN_9
#define BTN_ENTR_GPIO_Port GPIOB
#define PPM_Pin GPIO_PIN_1
#define PPM_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
