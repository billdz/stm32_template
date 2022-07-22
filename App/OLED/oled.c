/*
		OLED �ߴ� 128x64
		纵向 分页， 共8页，每页8行
		横向 分列，每列中页的位置 按MSB在下
*/


#include "oled.h"


const unsigned char OLED_init_cmd[25]=
{
  /*0xae,0X00,0X10,0x40,0X81,0XCF,0xff,0xa1,0xa4,
  0xA6,0xc8,0xa8,0x3F,0xd5,0x80,0xd3,0x00,0XDA,0X12,
  0x8d,0x14,0xdb,0x40,0X20,0X02,0xd9,0xf1,0xAF*/
       0xAE,//关闭显示
       0xD5,//设置时钟分频因子,震荡频率
       0x80,  //[3:0],分频因子;[7:4],震荡频率
            
       0xA8,//设置驱动路数
       0X3F,//默认0X3F(1/64)
       0xD3,//设置显示偏移
       0X00,//默认为0
       0x40,//设置显示开始行 [5:0],行数.                              
       0x8D,//电荷泵设置
       0x14,//bit2，开启/关闭
       0x20,//设置内存地址模式
       0x02,//[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
       0xA1,//段重定义设置,bit0:0,0->0;1,0->127;
       0xC8,//设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
       0xDA,//设置COM硬件引脚配置
       0x12,//[5:4]配置            
       0x81,//对比度设置
       0xEF,//1~255;默认0X7F (亮度设置,越大越亮)
       0xD9,//设置预充电周期
       0xf1,//[3:0],PHASE 1;[7:4],PHASE 2;
       0xDB,//设置VCOMH 电压倍率
       0x30,//[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
       0xA4,//全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
       0xA6,//设置显示方式;bit0:1,反相显示;0,正常显示        
       0xAF,//开启显示     
};


// ��ʼ��IIC�˿�
void IIC_GPIO_Init(void)
{
#ifdef USE_STDPERIPH_DRIVER
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN | IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT,&GPIO_InitStructure);
	
	IIC_SCL_H();
	IIC_SDA_H();
#endif
}

void OLED_Send_Cmd(uint8_t cmd)
{
#ifdef USE_STDPERIPH_DRIVER
	IIC_Start();
	IIC_Write(OLED_ADDR);
	IIC_Write(0x00);
	IIC_Write(cmd);
	IIC_Stop();
#endif
#ifdef USE_HAL_DRIVER
	HAL_I2C_Mem_Write(&hi2c1, OLED_ADDR, (uint16_t)0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
#endif
}

void OLED_Send_Data(uint8_t data)
{
#ifdef USE_STDPERIPH_DRIVER
	IIC_Start();
	IIC_Write(OLED_ADDR);
	IIC_Write(0x40);
	IIC_Write(data);
	IIC_Stop();
#endif
#ifdef USE_HAL_DRIVER
	HAL_I2C_Mem_Write(&hi2c1, OLED_ADDR, (uint16_t)0x40, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
#endif
}

void OLED_Column_Set(uint8_t column)
{
	OLED_Send_Cmd(0x10|(column>>4));    //�����е�ַ��λ
	OLED_Send_Cmd(0x00|(column&0x0f));   //�����е�ַ��λ  
}

void OLED_Page_Set(uint8_t page)
{
	OLED_Send_Cmd(0xb0+page);
}

void OLED_Clear(void)
{
	uint8_t page,column;
	for(page=0;page<8;page++)             //page loop
	{ 
		OLED_Page_Set(page);
		OLED_Column_Set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			OLED_Send_Data(0x00);
		}
	}
}

void OLED_Full(void)
{
	uint8_t page,column;
	for(page=0;page<8;page++)             //page loop
	{ 
		OLED_Page_Set(page);
		OLED_Column_Set(0);	  
		for(column=0;column<128;column++)	//column loop
		{
			OLED_Send_Data(0xff);
		}
	}
}

void OLED_Init(void)
{
	IIC_GPIO_Init();
	uint8_t i;
	for(i=0;i<25;i++)
	{
		OLED_Send_Cmd(OLED_init_cmd[i]);
	}
}

void OLED_Display_Pic(const unsigned char *ptr_pic)
{
	uint8_t page,column;
	for(page=0;page<OLED_PAGE_MAX;page++)        //page loop
	{ 
		OLED_Page_Set(page);
		OLED_Column_Set(0);	  
		for(column=0;column<OLED_X_MAX;column++)	//column loop
		{
			OLED_Send_Data(*ptr_pic++);
		}
	}
}

void OLED_Display_ASCII_8x16(uint8_t page, uint8_t col, char ch)
{
	uint8_t i;
	if (ch < ' ' || ch > '~')
		ch = '~' + 1;
	int idx = (ch - ' ') * 16;
	const uint8_t * p = &ASCII_8x16[idx];
	OLED_Page_Set(page);
	OLED_Column_Set(col);
	for(i=0; i<8; i++)
	{
		OLED_Send_Data(*p++);
	}
	OLED_Page_Set(page+1);
	OLED_Column_Set(col);
	for(i=0; i<8; i++)
	{
		OLED_Send_Data(*p++);
	}
}

/*******************************************************************************
* �� �� ��       : OLED_Display_String
* ��������		 	 : ��ʾASCII8x16�ַ���(��'\0'���з�������
* ��    ��       : ҳ���У��ַ���
* ��    ��    	 : ��
*******************************************************************************/
void OLED_Display_String(uint8_t page, uint8_t col, char * str)
{
	uint8_t ch;
	uint8_t col0 = col;
	while(*str != 0x00 && *str != 0x0A && *str != 0x0D)
	{
		ch = *str++;
		OLED_Display_ASCII_8x16(page, col, ch);
		col += 8;
		if (col+8 > OLED_X_MAX)
		{
			page += 2;
			col = col0;
		}
		if (page+2 > OLED_PAGE_MAX)
			break;
	}
}

