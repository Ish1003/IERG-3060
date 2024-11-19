#include "stm32f10x.h"

void IERG3810_clock_tree_Init(void);
void IERG3810_USART1_Init(u32 pclk1, u32 bound);
void Delay(u32 count);
void USART_print(u8 USARTport, char *st);

int main(void)
{
  IERG3810_clock_tree_Init();
	IERG3810_USART1_Init(72, 9600);
	Delay(72000000);
  while (1)
  {
		USART_print(1, "Hello, World!");
  }
}

void Delay(u32 count)
{
	u32 i;
	for (i = 0; i < count; i++);
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

void IERG3810_USART1_Init(u32 pclk1, u32 bound)
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
	RCC->APB2ENR |= 1 << 14;
	GPIOA->CRH &= 0xFFFFF00F;
	GPIOA->CRH |= 0x000008B0;
	RCC->APB2RSTR |= 1 << 14;
	RCC->APB2RSTR &= ~(1 << 14);
	USART1->BRR = mantissa;
	USART1->CR1 |= 0x2008;
}

void USART_print(u8 USARTport, char *st)
{
	u8 i = 0;
	while(st[i] != 0x00) {
		if(USARTport == 1) {
			USART1->DR = st[i];
			while(!(USART1->SR & 0x00000080));// loop until TXE, which is USART->SR[7] is 1
		}
		if(USARTport == 2) {
			USART2->DR = st[i];
			while(!(USART2->SR & 0x00000080));
		}
		if(i == 255) break;
		i ++;
	}
}

