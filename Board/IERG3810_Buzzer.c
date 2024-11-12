#include "stm32f10x.h"
#include "IERG3810_Buzzer.h"

void IERG3810_Buzzer_Init()
{
	RCC ->APB2ENR |= 1 << 3;
	GPIOB->CRH&=0xFFFFFFF0;
	GPIOB->CRH |=3;
}
