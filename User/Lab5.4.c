#include "stm32f10x.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_NVIC.h"
#include "IERG3810_LED.h"

void IERG3810_TIM3_Init(u16 arr, u16 psc);

void TIM3_IRQHandler(void);

void DS0_turnOff();
void DS0_turnOff2(void);

u8 task1HeartBeat;
u8 task2HeartBeat;

int main(void)
{
	IERG3810_clock_tree_Init();
	IERG3810_LED_Init();
	
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_TIM3_Init(4999, 7199);
	GPIOB->BRR = 1 << 5;
	while(1)
	{
		//GPIOB->BRR = 1 << 5;
		GPIOB->BSRR = 1 << 5;
		//DS0_turnOff();
		//DS0_turnOff2();
	}
}

void IERG3810_TIM3_Init(u16 arr, u16 psc)
{
	RCC->APB1ENR|=1<<1;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->DIER|=1<<0;
	TIM3->CR1|=0x01;
	NVIC->IP[29] = 0x45;
	NVIC->ISER[0] |= (1 << 29);
}

void TIM3_IRQHandler(void)
{
	if (TIM3->SR & 1<<0)
	{
		GPIOB->ODR ^= 1<<5;
	}
	TIM3->SR &= ~(1<<0);
}

void DS0_turnOff()
{
	//level one subroutine;
	GPIOB->BSRR = 1 << 5;
}

void DS0_turnOff2(void)
{
	//level two subroutine;
	DS0_turnOff();
}
