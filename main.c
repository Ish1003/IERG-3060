#include "stm32f10x.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_USART.h"
#include "IERG3810_NVIC.h"

void IERG3810_key2_ExtiInit(void);

u8 task1HeartBeat;
u8 task2HeartBeat;

int main(void)
{
	IERG3810_clock_tree_Init();
	IERG3810_USART2_Init(36, 9600);
	IERG3810_Buzzer_Init();
	IERG3810_TFTLCD_Init();
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key2_ExtiInit();
	
	while (1)
	{
		
	}
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

void IERG3810_key2_ExtiInit(void)
{
	//key2 -- exti-2
	RCC->APB2ENR |= 1 << 6;
	GPIOE->CRL &= 0xFFFFF0FF;
	GPIOE->CRL |= 0x00000800;
	GPIOE->ODR |= 1 << 2;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFFF0FF;
	AFIO->EXTICR[0] |= 0x00000400;
	EXTI->IMR |= 1 << 2;
	EXTI->FTSR |= 1 << 2;
	 // EXTI->RTSR |= 1 << 2;
	
	NVIC->IP[8] = 0x65;  //priority
	NVIC->ISER[0] &= ~(1<<8);
	
	NVIC->ISER[0] |= (1<<8);

}

void EXTI2_IRQHandler(void)
{
	u8 i;
	for(i = 0; i < 10; i++)
	{
		//DS0 on
		GPIOB->BRR |= 1 << 5;
		Delay(1000000);
		//DS0 off
		GPIOB->BSRR |= 1 << 5;
		Delay(1000000);
	}
	EXTI->PR = 1 << 2;
}
