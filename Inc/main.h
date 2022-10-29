/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#define KEY2_Pin GPIO_PIN_2
#define KEY2_GPIO_Port GPIOE
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define T_CS_Pin GPIO_PIN_13
#define T_CS_GPIO_Port GPIOC
#define GBC_KEY_Pin GPIO_PIN_6
#define GBC_KEY_GPIO_Port GPIOF
#define LIGHT_SENSOR_Pin GPIO_PIN_7
#define LIGHT_SENSOR_GPIO_Port GPIOF
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOF
#define BREATH_Pin GPIO_PIN_9
#define BREATH_GPIO_Port GPIOF
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOF
#define IMU_INT_Pin GPIO_PIN_0
#define IMU_INT_GPIO_Port GPIOC
#define WKUP_Pin GPIO_PIN_0
#define WKUP_GPIO_Port GPIOA
#define DBG_T_Pin GPIO_PIN_2
#define DBG_T_GPIO_Port GPIOA
#define DBG_R_Pin GPIO_PIN_3
#define DBG_R_GPIO_Port GPIOA
#define TPAD_Pin GPIO_PIN_5
#define TPAD_GPIO_Port GPIOA
#define T_SCK_Pin GPIO_PIN_0
#define T_SCK_GPIO_Port GPIOB
#define T_PEN_Pin GPIO_PIN_1
#define T_PEN_GPIO_Port GPIOB
#define T_MISO_Pin GPIO_PIN_2
#define T_MISO_GPIO_Port GPIOB
#define T_MOSI_Pin GPIO_PIN_11
#define T_MOSI_GPIO_Port GPIOF
#define F_CS_Pin GPIO_PIN_14
#define F_CS_GPIO_Port GPIOB
#define LCD_BL_Pin GPIO_PIN_15
#define LCD_BL_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
