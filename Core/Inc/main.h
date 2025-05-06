/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define AIN1_Pin GPIO_PIN_2
#define AIN1_GPIO_Port GPIOE
#define AIN2_Pin GPIO_PIN_3
#define AIN2_GPIO_Port GPIOE
#define BIN1_Pin GPIO_PIN_4
#define BIN1_GPIO_Port GPIOE
#define BIN2_Pin GPIO_PIN_5
#define BIN2_GPIO_Port GPIOE
#define CIN1_Pin GPIO_PIN_6
#define CIN1_GPIO_Port GPIOE
#define Encoder2A_Pin GPIO_PIN_0
#define Encoder2A_GPIO_Port GPIOA
#define Encoder2B_Pin GPIO_PIN_1
#define Encoder2B_GPIO_Port GPIOA
#define servo3_Pin GPIO_PIN_6
#define servo3_GPIO_Port GPIOA
#define BEE_Pin GPIO_PIN_7
#define BEE_GPIO_Port GPIOA
#define Enconder1A_Pin GPIO_PIN_9
#define Enconder1A_GPIO_Port GPIOE
#define Enconder1B_Pin GPIO_PIN_11
#define Enconder1B_GPIO_Port GPIOE
#define CIN2_Pin GPIO_PIN_12
#define CIN2_GPIO_Port GPIOE
#define DIN1_Pin GPIO_PIN_13
#define DIN1_GPIO_Port GPIOE
#define DIN2_Pin GPIO_PIN_14
#define DIN2_GPIO_Port GPIOE
#define KEY1_Pin GPIO_PIN_12
#define KEY1_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_13
#define KEY2_GPIO_Port GPIOB
#define KEY3_Pin GPIO_PIN_14
#define KEY3_GPIO_Port GPIOB
#define KEY4_Pin GPIO_PIN_15
#define KEY4_GPIO_Port GPIOB
#define Enconder4A_Pin GPIO_PIN_12
#define Enconder4A_GPIO_Port GPIOD
#define Enconder4B_Pin GPIO_PIN_13
#define Enconder4B_GPIO_Port GPIOD
#define PWM1_Pin GPIO_PIN_6
#define PWM1_GPIO_Port GPIOC
#define PWM2_Pin GPIO_PIN_7
#define PWM2_GPIO_Port GPIOC
#define PWM3_Pin GPIO_PIN_8
#define PWM3_GPIO_Port GPIOC
#define PWM4_Pin GPIO_PIN_9
#define PWM4_GPIO_Port GPIOC
#define Enconder3A_Pin GPIO_PIN_4
#define Enconder3A_GPIO_Port GPIOB
#define Enconder3B_Pin GPIO_PIN_5
#define Enconder3B_GPIO_Port GPIOB
#define servo1_Pin GPIO_PIN_8
#define servo1_GPIO_Port GPIOB
#define servo2_Pin GPIO_PIN_9
#define servo2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
