#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"


void SysTick_Init(void);
void Delay_10us(__IO uint32_t nTime);

// 在SysTick中断服务函数SysTick_Handler()中调用
void TimingDelay_Decrement(void);

#endif
