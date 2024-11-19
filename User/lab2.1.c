#include "stm32f10x.h"

void IERG3810_clock_tree_Init(void);
void IERG3810_USART2_Init(u32 pclk1, u32 bound);
void Delay(u32 count);
	
int main(void)
{
  IERG3810_clock_tree_Init();
	IERG3810_USART2_Init(36, 9600);
	
  while (1)
  {
		USART2->DR = 0x41;
		Delay(50000);
		USART2->DR = 0x42;
		Delay(50000);
		Delay(1000000);
  }
}

void IERG3810_USART2_Init(u32 pclk1, u32 bound)
{
	float temp;
	u16 mantissa;
	u16 fraction;
	temp = (float)(pclk1 * 1000000) / (bound * 16);
	mantissa = temp;
	fraction = (temp - mantissa) * 16;
	mantissa <<= 4;
	mantissa += fraction;
	RCC->APB2ENR |= 1 << 2;
	RCC->APB1ENR |= 1 << 17;
	GPIOA->CRL &= 0xFFFF00FF;
  GPIOA->CRL |= 0x00008B00;
	RCC->APB1RSTR |= 1 << 17;
	RCC->APB1RSTR &= ~(1 << 17);
	USART2->BRR = mantissa;
	USART2->CR1 |= 0x2008;
}

void IERG3810_clock_tree_Init(void)
{
	u8 PLL = 7;
	unsigned char temp = 0;
	RCC->CFGR &= 0xF8FF0000;
	RCC->CR &= 0xFEF6FFFF;
	RCC->CR |= 0x00010000;
	while(!(RCC->CR>>17));
	RCC->CFGR = 0x00000400;
	RCC->CFGR |= PLL<<18;
	RCC->CFGR |= 1<<16;
	
	FLASH->ACR |= 0x32;
	RCC->CR |=0x01000000;
	while(!(RCC->CR>>25));
	RCC->CFGR |= 0x00000002;
	while(temp != 0x02)
	{
		temp = RCC->CFGR>>2;
		temp &= 0x03;
	}
}

void Delay(u32 count)
{
	u32 i;
	for (i = 0; i < count; i++);
}

