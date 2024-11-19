#include "stm32f10x_it.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_NVIC.h"
#include "IERG3810_LED.h"
#include "IERG3810_Clock.h"

void IERG3810_TIM3_Init(u16 arr, u16 psc);
void IERG3810_TIM4_Init(u16 arr, u16 psc);
void IERG3810_SYSTICK_Init10ms(void);
void IERG3810_TIM3_PwmInit(u16 arr, u16 psc);
void IERG3810_TIM3_PwmColorLEDInit(u16 arr, u16 psc);
void setColor(u8 r, u8 g, u8 b);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);

void function1(void) {
	setColor(1, 0, 0);
	Delay(15000000);
	setColor(1, 1, 0);
	Delay(15000000);
	setColor(0, 1, 0);
	Delay(15000000);
	setColor(0, 1, 1);
	Delay(15000000);
	setColor(0, 0, 1);
	Delay(15000000);
	setColor(1, 0, 1);
	Delay(15000000);
}

void function2(void) {
	Delay(15000000);
	setColor(1, 0, 0);
	Delay(15000000);
	setColor(0, 0, 0);
	Delay(15000000);
	setColor(1, 1, 0);
	Delay(15000000);
	setColor(0, 0, 0);
	Delay(15000000);
	setColor(0, 1, 0);
	Delay(15000000);
	setColor(0, 0, 0);
	Delay(15000000);
	setColor(0, 1, 1);
	Delay(15000000);
	setColor(0, 0, 0);
	Delay(15000000);
	setColor(0, 0, 1);
	Delay(15000000);
	setColor(0, 0, 0);
	Delay(15000000);
	setColor(1, 0, 1);
	Delay(15000000);
	setColor(0, 0, 0);
	Delay(15000000);
}

u8 task1HeartBeat;
u8 task2HeartBeat;

int main(void)
{
	u16 pc6pwmval = 0;
	u16 pc7pwmval = 0;
	u16 pc8pwmval = 0;

	u8 dir_r = 1;
	u8 dir_g = 1;
	u8 dir_b = 1;

	IERG3810_clock_tree_Init();
	IERG3810_LED_Init();
	IERG3810_TIM3_PwmColorLEDInit(9999, 72);
	
	while(1)
	{
		function1();
		/*Delay(5000);
		if (dir_r)
		{
			pc6pwmval ++;
		}
		else 
		{
			pc6pwmval --;
		}
		if (pc6pwmval > 5000) 
			dir_r=0;
		if (pc6pwmval ==0 && pc7pwmval ==0) 
			dir_r=1;
		
		if (dir_g)
		{
			pc7pwmval ++;
		}
		else 
		{
			pc7pwmval --;
		}
		if (pc7pwmval > 5000) 
			dir_g=0;
		if (pc7pwmval ==0 && pc6pwmval ==0) 
			dir_g=1;
		
		if (dir_b)
		{
			pc8pwmval ++;
		}
		else 
		{
			pc8pwmval --;
		}
		if (pc8pwmval > 5000) 
			dir_b=0;
		if (pc8pwmval ==0 && pc6pwmval ==0) 
			dir_b=1;
		
		TIM3->CCR1 = pc6pwmval;
		TIM3->CCR2 = pc7pwmval; //value tb compared
		TIM3->CCR3 = pc8pwmval;*/
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

void IERG3810_TIM3_PwmColorLEDInit(u16 arr, u16 psc)
{
	//PC6, 7, 8
	RCC->APB2ENR |= 1 << 4;
	GPIOC->CRL &= 0x00FFFFFF;
	GPIOC->CRL |= 0xBB000000;
	GPIOC->CRH &= 0xFFFFFFF0;
	GPIOC->CRH |= 0x0000000B;	
	
	RCC->APB2ENR |= 1 << 0;
	AFIO->MAPR &= 0xFFFFF3FF;
	AFIO->MAPR |= 3<<10; //remap
	RCC->APB1ENR |= 1 << 1;
	TIM3->ARR=arr;
	TIM3->PSC=psc;
	TIM3->CCMR1 |= 7 << 4;
	TIM3->CCMR1 |= 7 << 12; //OC2M 111, output compare 2 mode; 
	TIM3->CCMR2 |= 7 << 4;
	TIM3->CCMR1 |= 1 << 3;
	TIM3->CCMR1 |= 1 << 11; //OC2PE 1, output cmpr preload enable
	TIM3->CCMR2 |= 1 << 3;
	//channels enable
	TIM3->CCER |= 1<<0;
	TIM3->CCER |= 1<<4;
	TIM3->CCER |= 1<<8;
	TIM3->CR1 = 0x0080;
	TIM3->CR1 |=0x01;
}

void setColor(u8 r, u8 g, u8 b)
{
	TIM3->CCR1 = r ? 0:15000;
	TIM3->CCR2 = g ? 0:15000;
	TIM3->CCR3 = b ? 0:15000;
}
