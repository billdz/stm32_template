#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#ifdef USE_STDPERIPH_DRIVER
#include "stm32f10x.h"

#define SPI_FLASH_SPI               	SPI1
#define SPI_FLASH_SPI_CLK           	RCC_APB2Periph_SPI1
#define SPI_FLASH_SPI_SCK_PIN       	GPIO_Pin_5                  /* PA.05 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT 	GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_SCK_GPIO_CLK  	RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MISO_PIN      	GPIO_Pin_6                  /* PA.06 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT	GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MISO_GPIO_CLK 	RCC_APB2Periph_GPIOA
#define SPI_FLASH_SPI_MOSI_PIN      	GPIO_Pin_7                  /* PA.07 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT	GPIOA                       /* GPIOA */
#define SPI_FLASH_SPI_MOSI_GPIO_CLK 	RCC_APB2Periph_GPIOA
#define SPI_FLASH_CS_PIN            	GPIO_Pin_4                  /* PC.04 */
#define SPI_FLASH_CS_GPIO_PORT      	GPIOA                       /* GPIOC */
#define SPI_FLASH_CS_GPIO_CLK       	RCC_APB2Periph_GPIOA


#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)

#endif

#ifdef USE_HAL_DRIVER

#include "stm32f1xx_hal.h"
#include "main.h"
#include "spi.h"

#define Flash_spi				hspi1

#define SPI_FLASH_CS_LOW()		SPI1_CS_LOW()
#define SPI_FLASH_CS_HIGH()		SPI1_CS_HIGH()


#endif


void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif /* __SPI_FLASH_H */

