#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_clock.h"
#include "FONT.H"
#include "CFONT.H"

void IERG3810_TFTLCD_WrReg(u16 regval)
{
	LCD->LCD_REG = regval;
}

void IERG3810_TFTLCD_WrData(u16 data)
{
	LCD->LCD_RAM = data;
}

void LCD_Set9341_Parameter(void)
{
	IERG3810_TFTLCD_WrReg(0x01);
	IERG3810_TFTLCD_WrReg(0x11);
	
	IERG3810_TFTLCD_WrReg(0x3A);
	IERG3810_TFTLCD_WrData(0x55);
	
	IERG3810_TFTLCD_WrReg(0x29);
	
	IERG3810_TFTLCD_WrReg(0x36);
	IERG3810_TFTLCD_WrData(0xC8);
	
}

void IERG3810_TFTLCD_Init(void)
{
	RCC->AHBENR|= 1<<8;
	RCC->APB2ENR|= 1<<3;
	RCC->APB2ENR|= 1<<5;
	RCC->APB2ENR|= 1<<6;
	RCC->APB2ENR|= 1<<8;
	GPIOB->CRL&=0xFFFFFFF0;	
	GPIOB->CRL|=0x00000003;
	
	GPIOD->CRH&=0x00FFF000;
	GPIOD->CRH|=0xBB000BBB;
	GPIOD->CRL&=0xFF00FF00;
	GPIOD->CRL|=0x00BB00BB;
	
	GPIOE->CRH&=0x00000000;
	GPIOE->CRH|=0xBBBBBBBB;
	GPIOE->CRL&=0x0FFFFFFF;
	GPIOE->CRL|=0xB0000000;
	
	GPIOG->CRH&=0xFFF0FFFF;
	GPIOG->CRH|=0x000B0000;
	GPIOG->CRL&=0xFFFFFFF0;
	GPIOG->CRL|=0x0000000B;
	
	FSMC_Bank1->BTCR[6]=0x00000000;
	FSMC_Bank1->BTCR[7]=0x00000000;
	FSMC_Bank1E->BWTR[6]=0x00000000;
	FSMC_Bank1->BTCR[6]|= 1<<12;
	FSMC_Bank1->BTCR[6]|= 1<<14;
	FSMC_Bank1->BTCR[6]|= 1<<4;
	FSMC_Bank1->BTCR[7]|= 0<<28;
	FSMC_Bank1->BTCR[7]|= 1<<0;
	FSMC_Bank1->BTCR[7]|= 0xF<<8;
	FSMC_Bank1E->BWTR[6]|= 0<<28;
	FSMC_Bank1E->BWTR[6]|= 0<<0;
	FSMC_Bank1E->BWTR[6]|= 3<<8;
	FSMC_Bank1->BTCR[6]|= 1<<0;
	LCD_Set9341_Parameter();
	//LCD_LIGHT_ON;
	GPIOB->BSRR |= 1<<0;
}

void IERG3810_TFTLCD_DrawDot(u16 x, u16 y, u16 color)
{
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(x>>8);
		IERG3810_TFTLCD_WrData(x & 0xFF);
		IERG3810_TFTLCD_WrData(0x01);
		IERG3810_TFTLCD_WrData(0x3F);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(y>>8);
		IERG3810_TFTLCD_WrData(y & 0xFF);
		IERG3810_TFTLCD_WrData(0x01);
		IERG3810_TFTLCD_WrData(0xDF);
	IERG3810_TFTLCD_WrReg(0x2C);
	IERG3810_TFTLCD_WrData(color);
}

void IERG3810_TFTLCD_FillRectangle(u16 color, u16 start_x, u16 length_x, u16 start_y, u16 length_y)
{
	u32 index = 0;
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(start_x>>8);
		IERG3810_TFTLCD_WrData(start_x & 0xFF);
		IERG3810_TFTLCD_WrData((length_x + start_x -1) >> 8);
		IERG3810_TFTLCD_WrData((length_x + start_x -1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(start_y>>8);
		IERG3810_TFTLCD_WrData(start_y & 0xFF);
		IERG3810_TFTLCD_WrData((length_y + start_y -1) >> 8);
		IERG3810_TFTLCD_WrData((length_y + start_y -1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2C);
	for(index=0; index<length_x*length_y;index++)
	{
		IERG3810_TFTLCD_WrData(color);
	}
}

void IERG3810_TFTLCD_SevenSegment(u16 color, u16 start_x, u16 start_y, u8 digit)
{
	if(digit == 0 ||digit ==  2 ||digit ==  3 || digit == 5 || digit == 6 ||digit ==  7 ||digit == 8 || digit == 9)
	{	//a
		IERG3810_TFTLCD_FillRectangle(color, start_x - 140, 10,  start_y + 10, 55);
	}
	if(digit == 0||digit == 1||digit == 2||digit == 3||digit == 4||digit == 7||digit == 8||digit == 9)
	{	//b
		IERG3810_TFTLCD_FillRectangle(color, start_x - 130, 55, start_y + 65, 10);
	}
	if(digit == 0||digit == 1||digit == 3||digit == 4||digit == 5||digit == 6||digit == 7||digit == 8||digit == 9)
	{	//c
		IERG3810_TFTLCD_FillRectangle(color, start_x - 65, 55, start_y + 65, 10);
	}
	if(digit == 0||digit == 2||digit == 3||digit == 5||digit == 6||digit == 8||digit == 9)
	{	//d
	IERG3810_TFTLCD_FillRectangle(color, start_x - 10, 10, start_y + 10, 55);
	}
	if(digit == 0||digit == 2||digit == 6||digit == 8)
	{	//e
	IERG3810_TFTLCD_FillRectangle(color, start_x - 65, 55, start_y, 10);
	}
	if(digit == 0||digit == 4||digit == 5||digit == 6||digit == 8||digit == 9)
	{	//f
	IERG3810_TFTLCD_FillRectangle(color, start_x - 130, 55, start_y, 10);
	}
	if(digit == 2||digit == 3||digit == 4||digit == 5||digit == 6||digit == 8||digit == 9)
	{	//g
	IERG3810_TFTLCD_FillRectangle(color, start_x - 75, 10, start_y + 10, 55);
	}

}

void IERG3810_TFTCD_ShowChar(u16 x, u16 y, u8 ascii, u16 color, u16 bgcolor)
{
	u8 i, j;
	u8 index;
	u8 height = 16, length = 8;
	if(ascii < 32 || ascii >127) return;
	ascii -=32;
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(x>>8);
		IERG3810_TFTLCD_WrData(x & 0xFF);
		IERG3810_TFTLCD_WrData((length + x -1) >> 8);
		IERG3810_TFTLCD_WrData((length + x -1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(y>>8);
		IERG3810_TFTLCD_WrData(y & 0xFF);
		IERG3810_TFTLCD_WrData((height + y -1) >> 8);
		IERG3810_TFTLCD_WrData((height + y -1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2C);
	for(j=0; j<height/8; j++)
	{
		for(i=0; i<height/2; i++)
		{
			for(index=0; index<length; index++)
			{
				if((asc2_1608[ascii][index*2+1-j]>>i) & 0x01) IERG3810_TFTLCD_WrData(color);
				else IERG3810_TFTLCD_WrData(bgcolor);
			}
		}
	}
	
}

void IERG3810_TFTCD_ShowChinChar(u16 x, u16 y, u8 idx, u16 color, u16 bgcolor)
{
	u8 i, j;
	u8 index;
	u8 height = 16, length = 16;
	if(idx < 0 || idx >9) return;
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(x>>8);
		IERG3810_TFTLCD_WrData(x & 0xFF);
		IERG3810_TFTLCD_WrData((length + x -1) >> 8);
		IERG3810_TFTLCD_WrData((length + x -1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(y>>8);
		IERG3810_TFTLCD_WrData(y & 0xFF);
		IERG3810_TFTLCD_WrData((height + y -1) >> 8);
		IERG3810_TFTLCD_WrData((height + y -1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2C);
	for(j=0; j<height/8; j++)
	{
		for(i=0; i<height/2; i++)
		{
			for(index=0; index<length; index++)
			{
				if((chi_1616[idx][index*2+1-j]>>i) & 0x01) IERG3810_TFTLCD_WrData(color);
				else IERG3810_TFTLCD_WrData(bgcolor);
			}
		}
	}
	
}
