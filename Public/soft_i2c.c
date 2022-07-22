#include "soft_i2c.h"


//启动信号
//SCL在高电平期间，SDA由高电平向低电平的变化定义为启动信号
void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA_H();
	DELAY();
	IIC_SCL_H();
	DELAY();
	IIC_SDA_L();
	DELAY();DELAY();DELAY();
	IIC_SCL_L();//钳制IIC总线，准备发送或接受数据
}

//停止信号
//SCL在高电平期间，SDA由低电平向高电平的变化定义为停止信号
void IIC_Stop(void)
{
	IIC_SDA_L();
	DELAY();
	IIC_SCL_H();
	DELAY();DELAY();DELAY();
	IIC_SDA_H();
}

//写入  最后将SDA拉高，以等待从设备产生应答
void IIC_Write(uint8_t dat)
{
	uint8_t i, temp;
	temp = dat;
			
	for(i=0; i<8; i++)
	{	
		IIC_SCL_L();
		
		if ((temp&0x80)==0)
				IIC_SDA_L();
		else IIC_SDA_H();
		temp = temp << 1;
		DELAY();			  
		IIC_SCL_H();
		DELAY();
	}
	IIC_SCL_L();
	DELAY();
	IIC_SDA_H();
	DELAY();
	IIC_SCL_H();
//								不需要应答
//	if (SDA_VALUE==0)
//		ACK = 1;
//	else ACK =0;
	DELAY();
	IIC_SCL_L();
	DELAY();
}


