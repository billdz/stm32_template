/*
		基于stm32f10x单片机
*/

#ifndef __SOFT_I2C_H_
#define __SOFT_I2C_H_

#include "stm32f10x.h"
#include "delay.h"

#define DELAY()					

#define IIC_PORT				GPIOB
#define IIC_SCL_PIN			GPIO_Pin_6
#define IIC_SDA_PIN			GPIO_Pin_7

#define IIC_SCL_H()			GPIO_SetBits(IIC_PORT, IIC_SCL_PIN)
#define IIC_SDA_H()			GPIO_SetBits(IIC_PORT, IIC_SDA_PIN)
#define IIC_SCL_L()			GPIO_ResetBits(IIC_PORT, IIC_SCL_PIN)
#define IIC_SDA_L()			GPIO_ResetBits(IIC_PORT, IIC_SDA_PIN)

// SDA方向设置
#define SDA_IN() 				{GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |=(u32)8<<28;}
#define SDA_OUT() 			{GPIOB->CRL &= 0x0FFFFFFF;GPIOB->CRL |=(u32)3<<28;}

#define SDA_VALUE				(GPIOB->IDR & 0x0080)


void IIC_Start(void);
void IIC_Stop(void);
void IIC_Write(uint8_t dat);

#endif
