#include "stm32f10x.h"
#include "IERG3810_KEY.h"

void IERG3810_KEY_Init()
{
	RCC ->APB2ENR |= 1 << 2;
	GPIOA->CRL&=0xFFFFFFF0;
	GPIOA->CRL|=8;
	GPIOA->BRR |= 1;
	
	RCC ->APB2ENR |= 1 << 6;
	GPIOE->CRL&=0xFFF0FFFF;
	GPIOE->CRL|=((8)<<16);
	GPIOE->BSRR |= 1 << 4;
	
	GPIOE->CRL&=0xFFFF0FFF;
	GPIOE->CRL|=(8<<12);
	GPIOE->BSRR |= 1 << 3;
	
	GPIOE->CRL&=0xFFFFF0FF;
	GPIOE->CRL|=(8<<8);
	GPIOE->BSRR |= 1 << 2;
}
