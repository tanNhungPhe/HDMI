#ifndef HARDWARE_H_
#define HARDWARE_H_
#include <stdbool.h>
#include <stdint.h>
#include "gpio.h"

#define BUTTON_PRESS_MAX_CNTR 10
#define SoftI2C2_SDA_Pin GPIO_PIN_0
#define SoftI2C2_SDA_GPIO_Port GPIOB
#define SoftI2C2_SCL_Pin GPIO_PIN_1
#define SoftI2C2_SCL_GPIO_Port GPIOB
#define PWM_Pin GPIO_PIN_7
#define PWM_GPIO_Port GPIOB

// Define I2C Pin to TC chip
#define GPIO_Pin_SDA				GPIO_PIN_4
#define GPIO_Pin_SCL				GPIO_PIN_5
#define I2C_GPIO					GPIOB

#define TOSHIBA_I2C_ADDR 0x0F

#define USB_ON                  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
#define USB_OFF                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
#define TOSHIBA_RESET_OFF       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET)
#define TOSHIBA_RESET_ON        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET)
#define OLED_RESET              HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_SET)
#define OLED_SET                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9 | GPIO_PIN_10, GPIO_PIN_RESET)
#define MAIN_POWER_ON           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10 | GPIO_PIN_7 | GPIO_PIN_3, GPIO_PIN_SET)
#define MAIN_POWER_OFF          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10 | GPIO_PIN_7 | GPIO_PIN_3, GPIO_PIN_RESET)
#define LED_PWM_ON              HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2)
#define BUTTON_A_STATE          HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14)
#define BUTTON_B_STATE          HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15)

#define TOSHIBA_12_ON           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET)
#define TOSHIBA_12_OFF          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET)
#define TOSHIBA_18_ON           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET)
#define TOSHIBA_18_OFF          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET)

#define LED1_ON                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)
#define LED1_OFF                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET)
#define LED2_ON                 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET)
#define LED2_OFF                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET)
#define LED3_ON                 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET)
#define LED3_OFF                HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)
#define LED1_TOGGLE             HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
#define LED2_TOGGLE             HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
#define OLED_POWER_ON           HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_9, GPIO_PIN_SET)
#define OLED_POWER_OFF          HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_9, GPIO_PIN_RESET);

void MainPowerOn(void);
void DiasplayPowerOn(void);
//void PowerStage2to1(void);
void PowerOffStage2(void);
void MainPowerOff(void);
void ButtonsOn(void);
void Toshiba_Init(int edid_var);
void Stage2InitSplit(void);
void Led_A_Blink(int cicle, int mode);
void InitHardware(void);
void StatusControl(void);
void Buttons(void);

void DisplaySleep(void);
extern bool DisplayStatus;

#endif
