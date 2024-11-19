#include "stm32f10x_it.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_NVIC.h"
#include "IERG3810_LED.h"


void IERG3810_TIM3_Init(u16 arr, u16 psc);
void IERG3810_TIM4_Init(u16 arr, u16 psc);
void IERG3810_SYSTICK_Init10ms(void);

void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

u8 task1HeartBeat;
u8 task2HeartBeat;

int main(void)
{
	IERG3810_clock_tree_Init();
	IERG3810_LED_Init();
	
	IERG3810_NVIC_SetPriorityGroup(5);
	//IERG3810_TIM3_Init(4999, 7199);
	//IERG3810_TIM4_Init(1249, 7199);
	IERG3810_SYSTICK_Init10ms();
	task1HeartBeat = 0;
	task2HeartBeat = 0;	
	while(1)
	{
		if(task1HeartBeat >= 20)
		{
			//5 times per sec
			task1HeartBeat = 0;
			GPIOB->ODR ^= 1<<5;
		}
		if(task2HeartBeat >= 33)
		{
			//3 times per sec // 100/3 = 33.33
			task2HeartBeat = 0;
			GPIOE->ODR ^= 1<<5;
		}
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

void IERG3810_TIM4_Init(u16 arr, u16 psc)
{
	//IRQ
	RCC->APB1ENR|=1<<2;
	TIM4->ARR=arr;
	TIM4->PSC=psc;
	TIM4->DIER|=1<<0;
	TIM4->CR1|=0x01;
	NVIC->IP[30] = 0x35;
	NVIC->ISER[0] |= (1 << 30);
}

void TIM4_IRQHandler(void)
{
	if (TIM4->SR & 1<<0)
	{
		GPIOE->ODR ^= 1<<5;
	}
	TIM4->SR &= ~(1<<0);
}

void IERG3810_SYSTICK_Init10ms(void)
{
	//systick
	SysTick->CTRL = 0; //clear
	SysTick->LOAD = 720000/8 - 1; // 72 MHz = 72000000; 10 ms then 100 Hz, and then /8
	// CLKSOURCE = 0: STCLK (FCLK/8)
	// clock tree refers, pend handler
	SysTick->CTRL |= 0x03; // to be modified
	//set internal clk, use interrupt, start count
}


