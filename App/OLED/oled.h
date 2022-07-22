#ifndef	__OLED_H_
#define __OLED_H_

// 是否使用软件模拟I2C协议
#define USE_SOFT_I2C


#ifdef USE_STDPERIPH_DRIVER
#include "stm32f10x.h"
#ifdef USE_SOFT_I2C
#include "soft_i2c.h"
#endif
#endif

#ifdef USE_HAL_DRIVER
#include "stm32f1xx_hal.h"
#include "main.h"
#include "i2c.h"


#endif

#include "font.h"

#define OLED_ADDR		(0x78)

#define OLED_X_MAX		128
#define OLED_Y_MAX		64
#define OLED_PAGE_MAX 8
#define OLED_PAGE_ROW	8


void OLED_Init(void);
void OLED_Clear(void);
void OLED_Full(void);
void OLED_Display_Pic( const unsigned char *ptr_pic);
void OLED_Display_ASCII_8x16(uint8_t page, uint8_t col, char ch);
void OLED_Display_String(uint8_t page, uint8_t col, char * str);

#endif
