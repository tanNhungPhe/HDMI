#ifndef _TC2
#define _TC2

#include "main.h"
#include "iic.h"
#include "i2cSoft.h"

void Panel_Init_2(void);
void HDMI_Startup(void);
void Debug_Check_GPIO_And_Registers(void);

void TC358870_Enable_ColorBar(void);
#endif
