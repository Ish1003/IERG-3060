#include "stm32f10x.h"
#include "IERG3810_LED.h"

void IERG3810_LED_Init()
{
	RCC ->APB2ENR |= 1 << 3;
	GPIOB->CRL&=0xFF0FFFFF;
	GPIOB->CRL|=(3<<20);
	GPIOB->BSRR |= 1 << 5;
	
	RCC ->APB2ENR |= 1 << 6;
	GPIOE->CRL&=0xFF0FFFFF;
	GPIOE->CRL |=(3<<20);
	GPIOE->BSRR |= 1 << 5;
}
