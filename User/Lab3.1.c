#include "stm32f10x.h"

void Delay(u32 count)
{
	u32 i;
	for (i = 0; i < count; i++);
}

typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
}LCD_TypeDef;

#define LCD_BASE ((u32)(0x6C000000 | 0x000007FE))
#define LCD ((LCD_TypeDef *) LCD_BASE)


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

void LCD_Set7789_Parameter(void)
{
	IERG3810_TFTLCD_WrReg(0x01);
					Delay(1000000);
	IERG3810_TFTLCD_WrReg(0x11);
		Delay(1000000);	
	IERG3810_TFTLCD_WrReg(0x3A);
			IERG3810_TFTLCD_WrData(0x05);
	IERG3810_TFTLCD_WrReg(0x29);
	IERG3810_TFTLCD_WrReg(0x36);
			IERG3810_TFTLCD_WrData(0x80);
	
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


int main(void)
{
	u16 colors[5] = {0x0000, 0xFFFF, 0x07E0, 0xF800, 0x001F};
	u16 y_val[5] = {10, 20, 30, 40, 50};
	int i=0;
	int j=0;
	IERG3810_TFTLCD_Init();
	while(1)
	{
		//LCD: 320*480
		for(i=0; i<20; i++)
		{
			for(j=0; j<5; j++)
			{
				IERG3810_TFTLCD_DrawDot(10 + i, y_val[j], colors[j]);
				Delay(3000000);
			}
		}
	}
}
