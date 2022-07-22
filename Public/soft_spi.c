#include "soft_spi.h"


void SPI_Write(uint8_t data)
{
	uint8_t cnt;
	for(cnt=0; cnt<8; cnt++)
	{
		SPI_CLK_L();
		DELAY();
		if (data & 0x80) {
			SPI_MOSI_H();
		}
		else {
			SPI_MOSI_L();
		}
		data <<= 1;
		DELAY();
		SPI_CLK_H();
		DELAY();
	}
}
	
uint8_t SPI_Read(void)
{
	uint8_t cnt;
	uint8_t data = 0;
	
	for(cnt=0; cnt<8; cnt++)
	{
		SPI_CLK_L();
		DELAY();
		data <<= 1;
		if (SPI_MISO_VALUE) {
			data |= 0x01;
		}
		SPI_CLK_H();
		DELAY();
	}
	return data;
}

