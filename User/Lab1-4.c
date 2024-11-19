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
	GPIOB->BRR= 1 << 5;

}

void PA0_Config()
{
	//
	RCC ->APB2ENR |= 1 << 2;
	GPIOA->CRL&=0xFFFFFFF0;
	GPIOA->CRL|=8;
	GPIOA->BRR = 1;

}

void PE5_Config()
{
	//
	RCC ->APB2ENR |= 1 << 6;
	GPIOE->CRL&=0xFF0FFFFF;
	GPIOE->CRL|=(3<<20);
	GPIOE->BRR= 1 << 5;
}

void PE4_Config()
{
	//
	GPIOE->CRL&=0xFFF0FFFF;
	GPIOE->CRL|=((8)<<16);
	GPIOE->BSRR= 1 << 4;
}

void PE3_Config()
{
	//
	GPIOE->CRL&=0xFFFF0FFF;
	GPIOE->CRL|=(8<<12);
	GPIOE->BSRR= 1 << 3;
}

void PE2_Config()
{
	//
	GPIOE->CRL&=0xFFFFF0FF;
	GPIOE->CRL|=(8<<8);
	GPIOE->BSRR= 1 << 2;
}

void PB8_Config()
{
	//
	RCC ->APB2ENR |= 1 << 3;
	GPIOB->CRH&=0xFFFFFFF0;
	GPIOB->CRH|=3;
	//GPIOB->BSRR= 1 << 8;
}

int main(void)
{
	u32 PE3_prev_state = 0;
	u32 PE3_state = 0;
	u32 PA0_prev_state = 0;
	u32 PA0_state = 0;
	u32 count = 400000;
	u32 i = count;
	u32 j = count;
	
	PA0_Config();
  PB5_Config();
	PE5_Config();	PE4_Config();	PE3_Config();	PE2_Config();
	PB8_Config();
	

	PE3_prev_state = GPIOE->IDR & 0x00000008;
	PA0_prev_state = GPIOA->IDR & 0x00000001;
  while (1)
  {
		// LED0 and key2
		if((GPIOE->IDR) & 0x00000004)
		{
			GPIOB->ODR |=  1 << 5;
		}
		else
		{
			GPIOB->ODR &= ~ (1<<5);
		}

		//LED1 and key1
		PE3_state = GPIOE->IDR & 0x00000008; //E3
		if( PE3_state ^ PE3_prev_state) //if different
		{
			if (i >= count){
				i = 0;
				GPIOE->ODR ^= 1 << 5;
			}
			PE3_prev_state = PE3_state;
		}
		
		//buzzer and keyup
		PA0_state = GPIOA->IDR & 0x00000001;
		if( PA0_state ^ PA0_prev_state)
		{
			if (j >= count){
				j = 0;
				GPIOB->ODR  ^=  1<<8;
			}
			PA0_prev_state = PA0_state;
		}
		
		i++;
		j++;
  }
}

