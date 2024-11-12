#ifndef __IERG3810_TFTLCD_H
#define __IERG3810_TFTLCD_H

typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
}LCD_TypeDef;

#define LCD_BASE ((u32)(0x6C000000 | 0x000007FE))
#define LCD ((LCD_TypeDef *) LCD_BASE)

void IERG3810_TFTLCD_WrReg(u16 regval);
void LCD_Set9341_Parameter(void);
void IERG3810_TFTLCD_Init(void);
void IERG3810_TFTLCD_DrawDot(u16 x, u16 y, u16 color);
void IERG3810_TFTLCD_FillRectangle(u16 color, u16 start_x, u16 length_x, u16 start_y, u16 length_y);
void IERG3810_TFTLCD_SevenSegment(u16 color, u16 start_x, u16 start_y, u8 digit);
void IERG3810_TFTCD_ShowChar(u16 x, u16 y, u8 ascii, u16 color, u16 bgcolor);
void IERG3810_TFTCD_ShowChinChar(u16 x, u16 y, u8 idx, u16 color, u16 bgcolor);
#endif


