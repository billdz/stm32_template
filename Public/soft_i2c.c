#include "soft_i2c.h"


//�����ź�
//SCL�ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽ��͵�ƽ�ı仯����Ϊ�����ź�
void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA_H();
	DELAY();
	IIC_SCL_H();
	DELAY();
	IIC_SDA_L();
	DELAY();DELAY();DELAY();
	IIC_SCL_L();//ǯ��IIC���ߣ�׼�����ͻ��������
}

//ֹͣ�ź�
//SCL�ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ߵ�ƽ�ı仯����Ϊֹͣ�ź�
void IIC_Stop(void)
{
	IIC_SDA_L();
	DELAY();
	IIC_SCL_H();
	DELAY();DELAY();DELAY();
	IIC_SDA_H();
}

//д��  ���SDA���ߣ��Եȴ����豸����Ӧ��
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
//								����ҪӦ��
//	if (SDA_VALUE==0)
//		ACK = 1;
//	else ACK =0;
	DELAY();
	IIC_SCL_L();
	DELAY();
}


