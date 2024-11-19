#include "stm32f10x.h"

void Delay(u32 count)
{
	u32 i;
	for (i = 0; i < count; i++);
}

void PB5_Config()
{
	//
	RCC ->APB2ENR |= 1 << 3;
	GPIOB->CRL&=0xFF0FFFFF;
	GPIOB->CRL|=(3<<20);

}

int main(void)
{
  PB5_Config();
  while (1)
  {
		GPIOB->BRR = 1 << 5;
		Delay(8000000);
		GPIOB->BSRR = 1 << 5;
		Delay(8000000);
  }
}

