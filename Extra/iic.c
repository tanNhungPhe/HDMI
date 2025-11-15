#include "iic.h"

HAL_StatusTypeDef ylj=HAL_OK;

void i2cReadBytes(unsigned short DevID,unsigned short Addr,unsigned char *Pbuf,unsigned short Num)
{
	if(HAL_I2C_Mem_Read(&hi2c1,DevID,Addr,sizeof(Addr),Pbuf,Num,0xff)==HAL_BUSY)
	{
		HAL_I2C_DeInit(&hi2c1);//iic error,deinit
		HAL_I2C_Init(&hi2c1); 
		
		i2cReadBytes(DevID,Addr,Pbuf,Num);
	}
}

void i2cWriteBytes(unsigned short DevID,unsigned short Addr,unsigned char *Dat,unsigned short Num)
{
	if((ylj=HAL_I2C_Mem_Write(&hi2c1,DevID,Addr,sizeof(Addr),Dat,Num,0xff))==HAL_BUSY)
	{
		HAL_I2C_DeInit(&hi2c1);//iic error,deinit
		HAL_I2C_Init(&hi2c1); 
		
		i2cWriteBytes(DevID,Addr,Dat,Num);
	}
}

void i2c1_uh2cd_write8(uint16_t add,uint8_t data)
{
	i2cWriteBytes(0x1e,add,&data,1);
}

void i2c1_uh2cd_write16(uint16_t add,uint16_t data)
{
	uint8_t buf[2];
	buf[0]=data;
	buf[1]=(data>>8);
	i2cWriteBytes(0x1e,add,buf,2);
}

void i2c1_uh2cd_write32(uint16_t add,uint32_t data)
{
	uint8_t buf[4];
	buf[0]=data;
	buf[1]=(data>>8);
	buf[2]=(data>>16);
	buf[3]=(data>>24);
	i2cWriteBytes(0x1e,add,buf,4);
}

uint8_t i2c1_uh2cd_read8(uint16_t add)
{
	uint8_t data;

	i2cReadBytes(0x1e,add,&data,1);
	return data;  
}

uint16_t i2c1_uh2cd_read16(uint16_t add)
{
	uint16_t data;

	i2cReadBytes(0x1e,add,(uint8_t *)(&data),2);
	return data;
}
