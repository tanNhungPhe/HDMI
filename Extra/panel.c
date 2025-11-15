#include "panel.h"
#include "iic.h"

void Panel_Init(void)
{
	i2c1_uh2cd_write16(0x0504,0x0015);		
	i2c1_uh2cd_write16(0x0504,0x00ff);
	
	i2c1_uh2cd_write16(0x0504,0x0015);		
	i2c1_uh2cd_write16(0x0504,0x01fb);
	
	HAL_Delay(30);
	
	i2c1_uh2cd_write16(0x0504,0x0015);		
	i2c1_uh2cd_write16(0x0504,0x00ff);
	
	i2c1_uh2cd_write16(0x0504,0x0015);		
	i2c1_uh2cd_write16(0x0504,0x08d3);
	
	i2c1_uh2cd_write16(0x0504,0x0015);		
	i2c1_uh2cd_write16(0x0504,0x0ed4);
	
	i2c1_uh2cd_write16(0x0504,0x0005); // DCSCMD_Q				
	i2c1_uh2cd_write16(0x0504,0x0011); // DCSCMD_Q				
	HAL_Delay(120);
}