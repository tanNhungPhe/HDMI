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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern uint8_t status;
extern uint8_t lane_status;
extern uint8_t old_status;
extern uint8_t stage;
extern uint8_t mode; //  0 - Nopmal func split mode, 1 - SBS mode ORIG edid, 2 - 1 - SBS mode OSVR edid, 3 - SBS mode CUSTOM edid
extern uint8_t edid;

extern uint8_t hid_ver; //Now 3 for OSVR
extern uint8_t video_detected;
extern uint8_t packet_num;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_TEST_Pin GPIO_PIN_13
#define LED_TEST_GPIO_Port GPIOC
#define E6_Pin GPIO_PIN_3
#define E6_GPIO_Port GPIOA
#define E2_Pin GPIO_PIN_4
#define E2_GPIO_Port GPIOA
#define LED_1_Pin GPIO_PIN_5
#define LED_1_GPIO_Port GPIOA
#define LED_2_Pin GPIO_PIN_6
#define LED_2_GPIO_Port GPIOA
#define E7_Pin GPIO_PIN_7
#define E7_GPIO_Port GPIOA
#define A_Pin GPIO_PIN_10
#define A_GPIO_Port GPIOB
#define D_Pin GPIO_PIN_11
#define D_GPIO_Port GPIOB
#define C_Pin GPIO_PIN_12
#define C_GPIO_Port GPIOB
#define RIGHT_BTN_Pin GPIO_PIN_14
#define RIGHT_BTN_GPIO_Port GPIOB
#define LEFT_BTN_Pin GPIO_PIN_15
#define LEFT_BTN_GPIO_Port GPIOB
#define E10_Pin GPIO_PIN_8
#define E10_GPIO_Port GPIOA
#define E4_Pin GPIO_PIN_9
#define E4_GPIO_Port GPIOA
#define E1_Pin GPIO_PIN_10
#define E1_GPIO_Port GPIOA
#define RESET_Pin GPIO_PIN_15
#define RESET_GPIO_Port GPIOA
#define I2C_SDA_Pin GPIO_PIN_4
#define I2C_SDA_GPIO_Port GPIOB
#define I2C_SCL_Pin GPIO_PIN_5
#define I2C_SCL_GPIO_Port GPIOB
#define E5_Pin GPIO_PIN_6
#define E5_GPIO_Port GPIOB
#define E8_Pin GPIO_PIN_7
#define E8_GPIO_Port GPIOB
#define E9_Pin GPIO_PIN_8
#define E9_GPIO_Port GPIOB
#define B_Pin GPIO_PIN_9
#define B_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
