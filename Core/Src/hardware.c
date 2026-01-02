#include "hardware.h"
#include "gpio.h"
#include "main.h"
#include "Toshiba_TC358870_china.h"
#include "i2cSoft.h"
#include "usbd_cdc_if.h"
#include <stdio.h>
#include <stdbool.h>

uint8_t button_a_is_pressed = 0;
uint8_t button_b_is_pressed = 0;
uint8_t button_a_press_cntr = 0;
uint8_t button_b_press_cntr = 0;

bool HMDready = false;
bool DisplayStatus = true;
bool OledNeedUpdate = false;
static uint8_t regs_dumped = 0;

void MainPowerOn(void)
{
    TOSHIBA_RESET_OFF;
    MAIN_POWER_ON;
    //OLED_POWER_OFF;
    //OLED_RESET;
    TOSHIBA_RESET_ON;
    USB_ON;
    HAL_Delay(150);
    ////LED_PWM_ON;
    TOSHIBA_RESET_OFF;
}



void Led_A_Blink(int cicle, int mode) // modes: 0 - off, 1 - on, 2 - blink
{
    switch (mode)
    {
    case 0:
        // osTimerStop(LedTimerHandle);
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_RESET);
        break;

    case 1:
        // osTimerStop(LedTimerHandle);
        HAL_GPIO_WritePin(LED_1_GPIO_Port, LED_1_Pin, GPIO_PIN_SET);
        break;
    case 2:
        // osTimerStop(LedTimerHandle);
        // osTimerStart(LedTimerHandle, cicle);
        break;
    }
}

void DisplayPowerOn(void)
{
   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
    OLED_POWER_ON;
    HAL_Delay(200);
    OLED_SET;
    HAL_Delay(20);
}



void DisplaySleep(void)
{
   // OLED_RESET;
    //Toshiba_TC358870_OLED_SleepChina();
    //OLED_POWER_OFF;
}

void PowerOffStage2(void)
{
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_9, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
    HAL_Delay(100);
}

void Toshiba_Init(int edid_var)
{
    /*
      EDID_ORIGINAL 2
      EDID_OCULUS 1
      EDID_OSVR 3
    */

    HAL_Delay(20);
    TOSHIBA_RESET_ON;
    HAL_Delay(40);
    TOSHIBA_RESET_OFF;
    HAL_Delay(4);
    Toshiba_TC358870_HDMI_SetupChina();
    EDID_Write(edid_var);
}

void Stage2InitSplit(void)
{
    // LED1_OFF;
    // Toshiba_TC358870_OLED_SetupChina();
    HAL_Delay(120);
    Toshiba_TC358870_OLED_SetupChina(0);
    // HAL_Delay(40);
    HAL_Delay(10);
}

void MainPowerOff(void)
{
    USB_OFF;
    MAIN_POWER_OFF;
}

void Buttons()
{
    if (stage == 6)
    {
        if (BUTTON_A_STATE == 0)
        {
            button_a_press_cntr++;
            if (button_a_press_cntr >= BUTTON_PRESS_MAX_CNTR)
            {
                button_a_press_cntr = 0;
                button_a_is_pressed = 1;
            }
        }
        else
        {
            if (button_a_is_pressed)
            {
                switch (target_mode)
                {
                case 0:
                    target_mode = 1;
                    break;

                case 1:
                    target_mode = 0;
                    break;
                }
                /*
                SerialPrint("\r\nMode changed to ");
                char a = 0;
                itoa(target_mode, &a, 2);
                SerialPrint(&a);
                SerialPrint("\r\n");
                */

                ModesSwitch();
                button_a_is_pressed = 0;
            }
            button_a_press_cntr = 0;
        }

        if (BUTTON_B_STATE == 0)
        {
            button_b_press_cntr++;
            if (button_b_press_cntr >= BUTTON_PRESS_MAX_CNTR)
            {
                button_b_press_cntr = 0;
                button_b_is_pressed = 1;
            }
        }
        else
        {
            if (button_b_is_pressed)
            {
                switch (target_edid)
                {
                case 0:
                    target_edid = 1;
                    break;

                case 1:
                    target_edid = 2;
                    break;

                case 2:
                    target_edid = 0;
                    break;
                }

                EdidSwitch();
                button_b_is_pressed = 0;
            }
            button_b_press_cntr = 0;
        }
    }
}

void StatusControl()
{
    if (stage == 6)
    {
        // switch (status & 0x80)
        switch (status)
        {
        case 0: // The HDMI 5V power supply was lost and the cable was pulled. 9F -> 1F -> 2F -> 3F -> 2F -> 3F -> 39 -> 28 (28 ~ 10ms)
            HMDready = false;
            stage = 1;
            // SSD1306_DrawLine(0, 31, 127, 31, 0);
            Led_A_Blink(1000, 2);
            break;

        case 0x01: // The HDMI 5V power supply was lost and the cable was pulled 9F -> 1F -> 2F -> 3F -> 2F -> 3F -> 39 -> 28 (28 ~ 10ms)
            HMDready = false;
            stage = 1;
            // SSD1306_DrawLine(0, 31, 127, 31, 0);
            Led_A_Blink(500, 2);
            break;

        case 0x19: // Disabled by software
            //DisplaySleep();
            HMDready = false;
            stage = 3;
            // SSD1306_DrawLine(0, 31, 127, 31, 0);
            Led_A_Blink(100, 2);
            break;

        case 0x28: // the cable was pulled out  9F -> 1F -> 2F -> 3F -> 2F -> 3F -> 39 -> 28 (28 ~ 10ms) делаем OLED_sleep и ждем status 0xAF тогда поднимаем
            //DisplaySleep();
            HMDready = false;
            stage = 3;
            // SSD1306_DrawLine(0, 31, 127, 31, 0);
            Led_A_Blink(100, 2);
            break;

        case 0x9F: // Normal operation
            if (!HMDready)
                HMDready = true;
            break;
        case 0x8F: // Normal operation
            if (!HMDready)
                HMDready = true;
            break;
        case 0x1F:
            break;

        case 0x08:
            HMDready = false;
            // SSD1306_DrawLine(0, 31, 127, 31, 0);
            Led_A_Blink(100, 2);
            break;

        case 0x2F:
            HMDready = false;
            Led_A_Blink(100, 2);
            // SSD1306_DrawLine(0, 31, 127, 31, 0);
            break;

        case 0x3F:
            HMDready = false;
            Led_A_Blink(100, 2);
            // SSD1306_DrawLine(0, 31, 127, 31, 0);
            break;

        case 0x39:
            HMDready = false;
            Led_A_Blink(100, 2);
            // SSD1306_DrawLine(0, 31, 127, 31, 0);
            break;

        default:
            // DisplaySleep();
            HMDready = false;
            stage = 1;
            Led_A_Blink(100, 2);
            // SSD1306_DrawLine(0, 31, 127, 31, 0);
            break;
        }
    }
}

void InitHardware()
{
    printf("InitHardWare\n");
    /* Debug: scan I2C bus to find devices (helps detect correct addresses) */
    i2cSoft_Scan();
    /* Immediately check for Toshiba device and dump registers for quick feedback */
    if (i2cSoft_DeviceExists(TOSHIBA_I2C_ADDR)) {
        printf("Toshiba device (0x%02X) detected - dumping registers now...\r\n", TOSHIBA_I2C_ADDR);
        Toshiba_DumpRegisters();
        /* If lanes are not enabled, try to enable MIPI/PLL and dump again */
        uint8_t lane_chk = TC358870_i2c_Read(0x0290);
        if (lane_chk == 0) {
            printf("Lane status 0x%02X -> attempting to enable MIPI\r\n", lane_chk);
            Toshiba_EnableMIPI();
            /* if still zero after first enable, try reset/power-cycle and retry */
            uint32_t lane_after = TC358870_i2c_Read32 ? 0 : 0; /* keep compiler quiet */
            uint8_t lane2 = TC358870_i2c_Read(0x0290);
            if (lane2 == 0) {
                printf("LANE still 0 after enable -> performing reset-and-retry\r\n");
                Toshiba_ResetPanelAndRetry();
            }
        }
    } else {
        printf("Toshiba device (0x%02X) NOT detected after scan\r\n", TOSHIBA_I2C_ADDR);
    }
    status = TC358870_i2c_Read(0x8520);

    // 0 - initial,
    // 1 - powerstage1,
    // 2 = 5V is ok,
    // 3 - toshiba init,
    // 4 - powerstage2,
    // 5 - display init

    switch (stage)
    {
    case 0:
        MainPowerOn();
        TOSHIBA_RESET_OFF;
        MAIN_POWER_ON;
        //OLED_POWER_OFF;
        //OLED_RESET;
        TOSHIBA_RESET_ON;
        USB_ON;
        HAL_Delay(150);
        ////LED_PWM_ON;
        TOSHIBA_RESET_OFF;
        printf("stage 0 End\n");
        Led_A_Blink(500, 2);
        stage = 1;
        break;

    case 1:
        if ((status & 0x01) == 1)
            stage = 2;
        printf("stage 1 End\n");
        break;

    case 2:
        Toshiba_Init(target_edid);
        Led_A_Blink(200, 2);
        stage = 3;
        printf("stage 2 End Toshiba Init\n");
        break;

    case 25:
        if ((status & 0x01) == 1)
        {
            TOSHIBA_RESET_ON;
            HAL_Delay(40);
            stage = 1;
        }
        break;

    case 3:
       if (status == 0x9f || status == 0x8f)
        {
            HAL_Delay(10);
            printf("stage 3 End \n");
            stage = 4;
        }
        break;

    case 4:
        printf("stage 4 Start DisplayPowerOn \n");
        DisplayPowerOn();
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
        //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
         //OLED_POWER_ON;
         HAL_Delay(200);
         //OLED_SET;
         HAL_Delay(20);
        HAL_GPIO_WritePin(E6_GPIO_Port, E6_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(E4_GPIO_Port, E4_Pin, GPIO_PIN_SET); // EN 1v8
        HAL_Delay(200);
        HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET); // RST Panel
        HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, GPIO_PIN_SET); // RST Panel
        HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, GPIO_PIN_SET);
        HAL_Delay(200);
        HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, GPIO_PIN_RESET);
        HAL_Delay(10);
        HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, GPIO_PIN_SET); // RST Panel
        HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, GPIO_PIN_SET);

        HAL_GPIO_WritePin(E10_GPIO_Port, E10_Pin, GPIO_PIN_SET); // VSP VSN
        Toshiba_TC358870_OLED_SetupChina(target_mode); // Mode: 0 split, 1 - double
          HAL_GPIO_WritePin(E5_GPIO_Port, E5_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(E8_GPIO_Port, E8_Pin, GPIO_PIN_SET);
        Led_A_Blink(50, 2);
        stage = 5;
        printf("stage 4 End \n");
        break;

    case 5:
        printf("stage 5 Start Toshiba_TC358870_OLED_SetupChina \n");
        //Toshiba_TC358870_OLED_SetupChina(target_mode); // Mode: 0 split, 1 - double
        //HAL_GPIO_WritePin(E5_GPIO_Port, E5_Pin, GPIO_PIN_SET);
        //HAL_GPIO_WritePin(E8_GPIO_Port, E8_Pin, GPIO_PIN_SET);
        HAL_Delay(240);

        stage = 6;
        mode = target_mode;
        edid = target_edid;
        Led_A_Blink(50, 1);
        printf("stage 5 End Toshiba_TC358870_OLED_SetupChina \n");
        lane_status = TC358870_i2c_Read(0x0290);
        uint8_t hdmi_int_get = TC358870_i2c_Read(0x850B);
        uint8_t sys_status_get = TC358870_i2c_Read(0x8520);

        uint16_t SYS_STATUS = TC358870_i2c_Read16 (0x0004) ;
        uint16_t CLK_STATUS = TC358870_i2c_Read16 (0x0006) ;
		uint8_t MIPI_LANE_STATUS = TC358870_i2c_Read (0x0110) ;
		uint8_t MIPI_FRAME_STATUS = TC358870_i2c_Read (0x0112) ;
		int E1_Status = HAL_GPIO_ReadPin(E1_GPIO_Port, E1_Pin); // 1v1  VDD_HDMI
		int E2_Status = HAL_GPIO_ReadPin(E2_GPIO_Port, E2_Pin); // 1v2  VDD_MIPI
		int E3_Status = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1); // 3v3  VDD33_HDMI VDDIO33
		int E4_Status = HAL_GPIO_ReadPin(E4_GPIO_Port, E4_Pin); // 1.8v VDD_DISP_18
		int E5_Status = HAL_GPIO_ReadPin(E5_GPIO_Port, E5_Pin); // VDD_BL (V3)
		int E6_Status = HAL_GPIO_ReadPin(E6_GPIO_Port, E6_Pin); // 3.3v VDD_DISP_33
		int E7_Status = HAL_GPIO_ReadPin(E7_GPIO_Port, E7_Pin); // VDD_OLED (V1/V2)
		int E8_Status = HAL_GPIO_ReadPin(E8_GPIO_Port, E8_Pin); // VDD_BL (V3)
		int E9_Status = HAL_GPIO_ReadPin(E9_GPIO_Port, E9_Pin); // Feedback
		int E10_Status = HAL_GPIO_ReadPin(E10_GPIO_Port, E10_Pin); // VDD_OLED (V1/V2)
        break;

    case 6:
        printf("stage 6 Start Toshiba_TC358870_OLED_SetupChina \n");
        lane_status = TC358870_i2c_Read(0x0290);
         hdmi_int_get = TC358870_i2c_Read(0x850B);
         sys_status_get = TC358870_i2c_Read(0x8520);

         SYS_STATUS = TC358870_i2c_Read16 (0x0004) ;
         CLK_STATUS = TC358870_i2c_Read16 (0x0006) ;
		 MIPI_LANE_STATUS = TC358870_i2c_Read (0x0110) ;
		 MIPI_FRAME_STATUS = TC358870_i2c_Read (0x0112) ;
        if (!HMDready)
        {

                OledNeedUpdate = true;
            HMDready = true;
        }

        /* Dump registers once for debugging */
        if (!regs_dumped) {
            Toshiba_DumpRegisters();
            regs_dumped = 1;
        }
        printf("stage 6 End Toshiba_TC358870_OLED_SetupChina \n");
        break;

    default:
        break;
    }
}
