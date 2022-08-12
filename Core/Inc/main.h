/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

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
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOC
#define BUTTON1_Pin GPIO_PIN_0
#define BUTTON1_GPIO_Port GPIOA
#define WS2812_N1_Pin GPIO_PIN_1
#define WS2812_N1_GPIO_Port GPIOA
#define WS2812_N2_Pin GPIO_PIN_2
#define WS2812_N2_GPIO_Port GPIOA
#define WS2812_SYNC_Pin GPIO_PIN_3
#define WS2812_SYNC_GPIO_Port GPIOA
#define WIZ_CS_Pin GPIO_PIN_4
#define WIZ_CS_GPIO_Port GPIOA
#define WIZ_SCK_Pin GPIO_PIN_5
#define WIZ_SCK_GPIO_Port GPIOA
#define WIZ_MISO_Pin GPIO_PIN_6
#define WIZ_MISO_GPIO_Port GPIOA
#define WIZ_MOSI_Pin GPIO_PIN_7
#define WIZ_MOSI_GPIO_Port GPIOA
#define WIZ_RESET_Pin GPIO_PIN_0
#define WIZ_RESET_GPIO_Port GPIOB
#define TFT_A0_Pin GPIO_PIN_1
#define TFT_A0_GPIO_Port GPIOB
#define TFT_CS_Pin GPIO_PIN_12
#define TFT_CS_GPIO_Port GPIOB
#define TFT_SCK_Pin GPIO_PIN_13
#define TFT_SCK_GPIO_Port GPIOB
#define TFT_SDI_Pin GPIO_PIN_15
#define TFT_SDI_GPIO_Port GPIOB
#define FLASH_SCL_Pin GPIO_PIN_6
#define FLASH_SCL_GPIO_Port GPIOB
#define FLASH_SDA_Pin GPIO_PIN_7
#define FLASH_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
