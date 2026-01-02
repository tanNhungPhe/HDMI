/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "i2cSoft.h"
#include "hardware.h"
#include<usbd_cdc_if.h>
#include <stdio.h>
#include <stdint.h>
#include "iic.h"
//#include "tc2.h"
#include "tc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t status = 0;
//0 - initial,
//1 - powerstage1,
//2 = 5V is ok,
//3 - toshiba init,
//4 - powerstage2,
//5 - display init
uint8_t stage = 0;
uint8_t new_stage = 2;
uint8_t edid = 0;
uint8_t target_edid = 0;
uint8_t target_mode = 0;
uint8_t old_status = 0;

//uint8_t state = 0;
// 1 - DS mode,
//2 - split mode,
//3 - HDMI off,
//4 - HDMI soft off
uint8_t lane_status = 0;
uint8_t mode = 0;
uint8_t hid_ver = 0;
uint8_t video_detected = 0;
uint8_t packet_num = 0;
uint8_t edid_ver = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int _write(int fd, char *str, int len) {
    CDC_Transmit_FS((uint8_t*)str, len); // USB 전송!
    return len;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void readTCchip(){

	  uint8_t buf[] = {0, 0, 0};
    buf[0]=0x00;
    buf[1]=0x00;
    if(i2cSoft_WriteBuffer ( TOSHIBA_I2C_ADDR, (uint8_t *)buf, 2 ) != 0)
          {
    	printf("\r\nI2C error - device not found");
                        return;
          }
           else if(i2cSoft_ReadBuffer ( TOSHIBA_I2C_ADDR, (uint8_t *)buf, 2 ) != 0)
                 {
        	   printf("\r\nI2C error - device not responding");
        	   return;
                  }

    char a[2];
    printf("Toshiba chip ID = 0x");
    itoa(buf[0], a, 16);
    printf(a);
    if(buf[0] == 0)printf(a);
    itoa(buf[1], a, 16);
    printf(a);
    if(buf[1] == 0)printf(a);
    printf("%s", a);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  /*----------i2c init----------*/
  //initI2C();
  //setAllPin();
  //lED INIT

// Stage0

 // HAL_GPIO_WritePin(A_GPIO_Port,A_Pin,GPIO_PIN_SET);// LCD RESET
  HAL_GPIO_WritePin(B_GPIO_Port,B_Pin,GPIO_PIN_SET);// LCD RESET
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); // E3: 3.3V HDMI (CRITICAL!)
  HAL_Delay(150);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); // E3: 3.3V HDMI (CRITICAL!)
  printf("Stage0 Init\n");
  MainPowerOn();
  TOSHIBA_RESET_OFF;
  MAIN_POWER_ON;
  //OLED_POWER_ON;  // Don't turn on OLED power yet, do it later
  //OLED_SET;
  TOSHIBA_RESET_ON;
  USB_ON;
  HAL_Delay(150);
  HAL_GPIO_WritePin(E5_GPIO_Port, E5_Pin, GPIO_PIN_SET);

  TOSHIBA_RESET_OFF;
  printf("Stage0 Done\n");
  printf("Stage1 Init\n");
  HAL_Delay(50);
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, 1);
  HAL_Delay(500);
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, 0);
  HAL_Delay(500);
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, 1);
  HAL_Delay(500);
  HAL_GPIO_WritePin(LED_2_GPIO_Port, LED_2_Pin, 0);
  HAL_Delay(500);

  i2cSoft_Init();
  HAL_Delay(150);
  while(1){
  if(i2cSoft_DeviceExists(TOSHIBA_I2C_ADDR))
	  printf("check TOSHIBA CHIP\n");
  break;
  }
  printf("Detect chip Tochiba\n");
  HAL_Delay(100);
  readTCchip();
  printf("Enable power rails\n");
  // Enable power rails
  HAL_GPIO_WritePin(E6_GPIO_Port, E6_Pin, GPIO_PIN_SET); // 3.3V Display
  HAL_GPIO_WritePin(E4_GPIO_Port,E4_Pin,GPIO_PIN_RESET);// E4: 1.8V Display
  HAL_GPIO_TogglePin(E7_GPIO_Port, E7_Pin);
  HAL_Delay(500);

  // tc2_init
  //tc2_init();

  tc_task();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //InitHardware();
	 // StatusControl();
	  HAL_GPIO_TogglePin(LED_1_GPIO_Port, LED_1_Pin);
	  HAL_Delay(1000);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void tc2_init(void)
{
  // Init begin
  // Enable Pluse for using digital analoger
  HAL_GPIO_WritePin(E4_GPIO_Port,E4_Pin,GPIO_PIN_SET);// E4: 1.8V Display
  HAL_Delay(500);
  HAL_GPIO_WritePin(E4_GPIO_Port,E4_Pin,GPIO_PIN_RESET);// E4: 1.8V Display
  HAL_Delay(1000);

  // enable LCD power
  HAL_Delay(10);
  HAL_GPIO_WritePin(E2_GPIO_Port,E2_Pin,GPIO_PIN_SET);//E2: 1.2V MIPI
  HAL_Delay(10);
  HAL_GPIO_WritePin(E4_GPIO_Port,E4_Pin,GPIO_PIN_SET);//E4: 1.8V Display
  HAL_Delay(120);
  
  // Enable Bias voltage for LCD
  HAL_GPIO_TogglePin(E10_GPIO_Port, E10_Pin);
  HAL_Delay(20);

  // LCD RESET
  HAL_GPIO_WritePin(B_GPIO_Port,B_Pin,GPIO_PIN_RESET);//LCD RESET
  HAL_Delay(100);

  // init sequence
  HDMI_Startup();
  HAL_Delay(1000);

  // Enable backlight
  HAL_GPIO_WritePin(E8_GPIO_Port,E8_Pin,GPIO_PIN_SET); //Backlight V3
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
