#include "stm32f10x_it.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_NVIC.h"
#include "IERG3810_LED.h"
#include "IERG3810_Clock.h"

void IERG3810_TIM3_Init(u16 arr, u16 psc);
void IERG3810_TIM4_Init(u16 arr, u16 psc);
void IERG3810_SYSTICK_Init10ms(void);
void IERG3810_TIM3_PwmInit(u16 arr, u16 psc);

	
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

u8 task1HeartBeat;
u8 task2HeartBeat;

int main(void)
{
	u16 led0pwmval = 0;
	u8 dir = 1;
	IERG3810_clock_tree_Init();
	IERG3810_LED_Init();
	
	//IERG3810_NVIC_SetPriorityGroup(5);
	//IERG3810_TIM3_Init(4999, 7199);
	//IERG3810_TIM4_Init(1249, 7199);
	//IERG3810_SYSTICK_Init10ms();

	IERG3810_TIM3_PwmInit(9999, 72); //9999=100, 6666-150
		IERG3810_TIM4_Init(1249, 7199);
	while(1)
	{
		Delay(1500);
		if (dir)led0pwmval ++;
		else led0pwmval --;
		if (led0pwmval > 5000) dir=0;
		if (led0pwmval ==0) dir=1;
		TIM3->CCR2 = led0pwmval; //value tb compared
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

void IERG3810_TIM3_PwmInit(u16 arr, u16 psc)
{
	RCC->APB2ENR |= 1 << 3;
	GPIOB->CRL &= 0xFF0FFFFF;
	GPIOB->CRL |= 0x00B00000;
	RCC->APB2ENR |= 1 << 0;
	AFIO->MAPR &= 0xFFFFF3FF;
	AFIO->MAPR |= 1<<11;
	RCC->APB1ENR |= 1 << 1;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->CCMR1 |= 7 << 12; //OC2M 111, output compare 2 mode; 
	TIM3->CCMR1 |= 1 << 11; //OC2PE 1, output cmpr preload enable
	TIM3->CCER |= 1<<4;
	TIM3->CR1 = 0x0080;
	TIM3->CR1 |=0x01;
}
