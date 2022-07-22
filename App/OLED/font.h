#ifndef	__FONT_H_
#define __FONT_H_
/*
	ASCII 8x16 纵向扫描，字节逆序
*/

#define CHAR_WIDTH		8
#define CHAR_HEIGHT		16
#define BYTE_PER_CHAR	16

/**
*@brief    !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~ 
*@help  start with ' ' 
*@usage    get [16] use char* p_font = FontAscii_8_16[(chToDisplay - ' ')*16];
*/
extern const unsigned char ASCII_8x16[];




#endif
