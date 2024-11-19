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
		for(i=0; i<10; i++)
		{
			IERG3810_TFTCD_ShowChar(80, 20 + 30 * i, 48 + sid[9 - i], 0x07FF, 0x0000);
		}
	}
}


