#include "SysTick.h"


// 延时程序中使用的计数器
uint32_t TimingDelay;

/*******************************************************************************
* 函 数 名       : SysTick_Init
* 函数功能		 : 启动系统滴答定时器 SySTick
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void SysTick_Init(void)
{
	/*
		SystemCoreClock / 1000		// 1ms中断一次
		SystemCoreClock / 100000	// 10us中断一次
		SystemCoreClock / 1000000	// 1us中断一次
	*/
	if (SysTick_Config(SystemCoreClock / 100000))	// 10us中断一次
	{	
		// 配置不成功时，进入死循环
		while (1);
	}
	// 关闭定时器
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

/*******************************************************************************
* 函 数 名       : Delay_10us
* 函数功能		 : 延时程序，10us为一个单位
* 输    入       : nTime  延时nTime个10us
* 输    出    	 : 无
* 说    明		 : Delay_10us(10) 实现延时 10 * 10us = 100us
*******************************************************************************/
void Delay_10us(__IO uint32_t nTime)
{
	TimingDelay = nTime;
	// 使能定时器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	
	while(TimingDelay != 0);
}

/*******************************************************************************
* 函 数 名       : TimingDelay_Decrement
* 函数功能		 : 递减计数器，实现节拍控制
* 输    入       : 无
* 输    出    	 : 无
* 说    明		 : 在SysTick中断服务函数SysTick_Handler()中调用
*******************************************************************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
