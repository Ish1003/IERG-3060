#include "stm32f10x.h"
#include "Name_and_SID.h"
#include "IERG3810_TFTLCD.h"

void name_and_SID(void)
{
	IERG3810_TFTLCD_FillRectangle(0xFFE0, 100, 100, 100, 100);
}
