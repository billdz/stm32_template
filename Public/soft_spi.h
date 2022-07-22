#ifndef __SOFT_SPI_H
#define __SOFT_SPI_H


#include "stm32f10x.h"
#include "delay.h"

#define DELAY()		

#define SPI_PORT	GPIOA
#define SPI_CS		GPIO_Pin_4
#define	SPI_CLK		GPIO_Pin_5
#define SPI_MISO	GPIO_Pin_6
#define	SPI_MOSI	GPIO_Pin_7

#define SPI_CLK_H()			GPIO_SetBits(SPI_PORT, SPI_CLK)
#define SPI_MOSI_H()		GPIO_SetBits(SPI_PORT, SPI_MOSI)
#define SPI_CS_H()			GPIO_SetBits(SPI_PORT, SPI_CS)

#define SPI_CLK_L()			GPIO_ResetBits(SPI_PORT, SPI_CLK)
#define SPI_MOSI_L()		GPIO_ResetBits(SPI_PORT, SPI_MOSI)
#define SPI_CS_L()			GPIO_ResetBits(SPI_PORT, SPI_CS)

#define SPI_MISO_VALUE	GPIO_ReadInputDataBit(SPI_PORT, SPI_MISO)

void 		SPI_Write(uint8_t data);
uint8_t	SPI_Read(void);



#endif
