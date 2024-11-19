#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_Clock.h"

int main(void)
{
	int i=0;
	u8 sid[10] = {1, 1, 5, 5, 1, 9, 1, 5, 9, 7};
	IERG3810_TFTLCD_Init();
	while(1)
	{
		/*for(i=0; i<6; i++)
		{
			IERG3810_TFTCD_ShowChinChar(32*i, 200, i, 0x07FF, 0x0000);
		}*/
		for(i=6; i<9; i++)
		{
			IERG3810_TFTCD_ShowChinChar(16*(i - 6), 150, i, 0x07FF, 0x0000);
		}
		for(i=0; i<10; i++)
		{
			IERG3810_TFTCD_ShowChar(8*i, 100, 48 + sid[i], 0x07FF, 0x0000);
		}
		
	}
}

