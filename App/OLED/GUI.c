

#include "GUI.h"
#include "string.h"

// 由于OLED是整页擦写，因此使用数据缓存将要写入的信息
#define GUI_CACHE_LEN		(OLED_X_MAX * OLED_PAGE_MAX)
static uint8_t OLED_Cache[GUI_CACHE_LEN] = {0};

#define SetBit(x, n)		x |= (0x01 << n)
#define RstBit(x, n)		x &= ~(0x01 << n)


/*******************************************************************************
* 函 数 名       : GUI_Init
* 函数功能		 	 : 初始化
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void GUI_Init(void)
{
	OLED_Init();
}

/*******************************************************************************
* 函 数 名       : GUI_Clear
* 函数功能		 	 : 清空屏幕
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void GUI_Clear(void)
{
	OLED_Clear();
	memset(OLED_Cache, 0x00, GUI_CACHE_LEN);
}

/*******************************************************************************
* 函 数 名       : GUI_Fill
* 函数功能		 	 : 填充整个屏幕
* 输    入       : color	： 颜色
* 输    出    	 : 无
*******************************************************************************/
void GUI_Fill(uint16_t color)
{
	OLED_Full();
	memset(OLED_Cache, 0xff, GUI_CACHE_LEN);
}

/*******************************************************************************
* 函 数 名       : GUI_Clear_Cache
* 函数功能		 	 : 清空缓存，但不改变显示
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void GUI_Clear_Cache(void)
{
	memset(OLED_Cache, 0x00, GUI_CACHE_LEN);
}

// 修改一个坐标在缓存数组中的对应位置的值
static void set_dot(uint8_t x, uint8_t y, uint8_t value)
{
	if (x >= OLED_X_MAX || y >= OLED_Y_MAX)
		return ;
	int m, n, index;
	m = y / OLED_PAGE_ROW;
	n = y % OLED_PAGE_ROW;
	index = m * OLED_X_MAX + x;
	if (value)
	{
		SetBit(OLED_Cache[index], n);
	}
	else
	{
		RstBit(OLED_Cache[index], n);
	}
}

// 根据缓存刷新显示
static void flush(void)
{
	OLED_Display_Pic(OLED_Cache);
}

/*******************************************************************************
* 函 数 名       : GUI_Show_Dot
* 函数功能		 	 : 显示一个点
* 输    入       : x, y	：位置		color	：颜色
* 输    出    	 : 无
*******************************************************************************/
void GUI_Show_Dot(uint8_t x, uint8_t y, uint16_t color)
{
	set_dot(x, y, color);
	flush();
}

// 生成直线，存储在缓存中，但不显示
static void gen_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color)
{
	int dx, dy,             // 坐标的差值
			dx2, dy2,           // dx,dy * 2
			x_inc, y_inc,       // 绘制时每次变化的像素点数         
			error,          // the discriminant i.e. error i.e. decision variable
			index;          

	dx = x1-x0;						//计算x距离
	dy = y1-y0;						//计算y距离

	if (dx>=0){
		x_inc = 1;
	}
	else{
		x_inc = -1;
		dx    = -dx;  
	} 
	if (dy>=0){
		y_inc = 1;
	} 
	else{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
	{//且线的点数等于x距离，以x轴递增画点
		// initialize error term
		error = dy2 - dx; 
		// 通过循环画线
		for (index=0; index <= dx; index++)//要画的点数不会超过x距离
		{
			//画点
			set_dot(x0, y0, color);
	
			// test if error has overflowed
			if (error >= 0) //是否需要增加y坐标值
			{
				error-=dx2;
				// move to next line
				y0+=y_inc;//增加y坐标值
			} 
			// adjust the error term
			error+=dy2;
			// move to the next pixel
			x0+=x_inc;//x坐标值每次画点后都递增1
		} 
	}
	else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
	{//以y轴为递增画点
		// initialize error term
		error = dx2 - dy; 
		// 循环画线
		for (index=0; index <= dy; index++)
		{
			// 画点
			set_dot(x0, y0, color);
			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;
				// move to next line
				x0+=x_inc;	// 增加x坐标
			} 
			// adjust the error term
			error+=dx2;
			// 移动到下一个y坐标
			y0+=y_inc;
		} 
	}
}

/*******************************************************************************
* 函 数 名       : GUI_Show_Line
* 函数功能		 	 : 显示一条直线
* 输    入       : x0, y0 ：起始坐标	x1, y1 ：结束坐标		color ：颜色
* 输    出    	 : 无
*******************************************************************************/
void GUI_Show_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color)
{
	gen_line(x0, y0, x1, y1, color);
	flush();
}


/*******************************************************************************
* 函 数 名       : GUI_Show_Rect
* 函数功能		 	 : 显示一个矩形框
* 输    入       : x, y ：左上角坐标	w, h ：宽、长		color ：颜色
* 输    出    	 : 无
*******************************************************************************/
void GUI_Show_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
{
	gen_line(x, y, x+w, y, color);
	gen_line(x+w, y+1, x+w, y+h, color);
	gen_line(x, y+h, x+w, y+h, color);
	gen_line(x, y, x, y+h, color);
	flush();
}

/*******************************************************************************
* 函 数 名       : GUI_Fill_Rect
* 函数功能		 	 : 填充指定矩形区域
* 输    入       : x, y ：左上角坐标	w, h ：宽、长		color ：颜色
* 输    出    	 : 无
*******************************************************************************/
void GUI_Fill_Rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color)
{
	uint8_t i, j;
	for(i=1; i<w; i++) {
		for(j=1; j<h; j++) {
			set_dot(x+i, y+j, color);
		}
	}
	flush();
}

// 生成圆形，存储数据到缓存，但不显示
void gen_circle(uint8_t X, uint8_t Y, uint8_t R, uint16_t fc)
{
	//Bresenham算法 
    uint8_t  a,b; 
    int c; 
    a=0; 
    b=R; 
    c=3-2*R; 
    while (a<b) 
    { 
        set_dot(X+a,Y+b,fc);     //        7 
        set_dot(X-a,Y+b,fc);     //        6 
        set_dot(X+a,Y-b,fc);     //        2 
        set_dot(X-a,Y-b,fc);     //        3 
        set_dot(X+b,Y+a,fc);     //        8 
        set_dot(X-b,Y+a,fc);     //        5 
        set_dot(X+b,Y-a,fc);     //        1 
        set_dot(X-b,Y-a,fc);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        set_dot(X+a,Y+b,fc); 
        set_dot(X-a,Y+b,fc); 
        set_dot(X+a,Y-b,fc); 
        set_dot(X-a,Y-b,fc); 
        set_dot(X+b,Y+a,fc); 
        set_dot(X-b,Y+a,fc); 
        set_dot(X+b,Y-a,fc); 
        set_dot(X-b,Y-a,fc); 
    } 
}

/*******************************************************************************
* 函 数 名       : GUI_Show_Circle
* 函数功能		 	 : 显示一个圆
* 输    入       : x, y ：圆心坐标	r ：半径	color ：颜色
* 输    出    	 : 无
*******************************************************************************/
void GUI_Show_Circle(uint8_t x, uint8_t y, uint8_t r, uint16_t color)
{
	gen_circle(x, y, r, color);
	flush();
}


// 生成圆形填充区域（不包含外圈），不显示
void gen_filled_circle(uint8_t x, uint8_t y, uint8_t r, uint16_t color)
{
	uint8_t  a,b; 
    int c; 
    a=0; 
    b=r-1; 
    c=3-2*(r-1); 
    while (a<b) 
    { 
				gen_line(x-a, y+b, x+a, y+b, color);
				gen_line(x-a, y-b, x+a, y-b, color);
				gen_line(x-b, y+a, x+b, y+a, color);
				gen_line(x-b, y-a, x+b, y-a, color);

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
				gen_line(x-a, y+b, x+a, y+b, color);
				gen_line(x-a, y-b, x+a, y-b, color);
				gen_line(x-b, y+a, x+b, y+a, color);
				gen_line(x-b, y-a, x+b, y-a, color);
    } 
}

/*******************************************************************************
* 函 数 名       : GUI_Fill_Circle
* 函数功能		 	 : 填充指定圆形区域
* 输    入       : x, y ：圆心坐标	r ：半径	color ：颜色
* 输    出    	 : 无
*******************************************************************************/
void GUI_Fill_Circle(uint8_t x, uint8_t y, uint8_t r, uint16_t color)
{
	gen_filled_circle(x, y, r, color);
	flush();
}


// 生成一个ASCII字符，不显示
void gen_char(uint8_t x, uint8_t y, char ch, uint16_t color)
{
	uint8_t i, j;
	uint8_t temp;
	if (ch < ' ' || ch > '~')		// 超出ASCII可显示码范围则显示为纯黑格
		ch = '~' + 1;
	int idx = (ch - ' ') * BYTE_PER_CHAR;
	const uint8_t * p = &ASCII_8x16[idx];
	for(i=0; i<CHAR_HEIGHT; i++)
	{
		temp = *p++;
		for(j=0; j<CHAR_WIDTH; j++)
		{
			uint8_t page = i % CHAR_WIDTH;
			uint8_t cross_page = i / CHAR_WIDTH;
			if( (temp>>j) & 0x01 )
				set_dot(x+page, y+j+OLED_PAGE_ROW*cross_page, color);
			else
				set_dot(x+page, y+j+OLED_PAGE_ROW*cross_page, 0);
		}
	}
}

/*******************************************************************************
* 函 数 名       : GUI_Show_String
* 函数功能		 	 : 显示ASCII字符串（以'\0'或回车符或换行符结尾）
* 输    入       : x, y	：起始字符左上角坐标	str ：字符串	color ：颜色
* 输    出    	 : 无
*******************************************************************************/
void GUI_Show_String(uint8_t x, uint8_t y, const char *str, uint16_t color)
{
	char ch;
	uint8_t x0 = x;
	while(*str != 0x00 && *str != 0x0A && *str != 0x0D)
	{
		ch = *str++;
		gen_char(x, y, ch, color);
		x += CHAR_WIDTH;
		if(x+CHAR_WIDTH > OLED_X_MAX)
		{
			y += CHAR_HEIGHT;
			x = x0;
		}
		if(y+CHAR_HEIGHT > OLED_Y_MAX)
			break;
		flush();
	}
}

/*******************************************************************************
* 函 数 名       : GUI_Show_Pic
* 函数功能		 	 : 显示图片（尺寸超出部分不显示）
* 输    入       : x, y ：左上角坐标	pic ：图片矩阵	w, h ：图片宽、长
* 输    出    	 : 无
*******************************************************************************/
void GUI_Show_Pic(uint8_t x, uint8_t y, 
				const uint16_t * pic, uint8_t w, uint8_t h)
{
	uint8_t i, j;
	uint8_t imax = w > OLED_X_MAX ? OLED_X_MAX : w;
	uint8_t jmax = h > OLED_Y_MAX ? OLED_Y_MAX : h;
	for(j=0; j<jmax; j++)
	{
		for(i=0; i<imax; i++)
		{
			set_dot(x+i, y+j, *pic++);
		}
	}
	flush();
}


