#include "stm32f10x.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_USART.h"
#include "IERG3810_NVIC.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"

void IERG3810_SYSTICK_Init1ms(void);
void name_and_SID(void);
void main_menu(void);
void launch_game(void);
void select_restart(void);


u8 state=0;
u8 task1HeartBeat;
u8 task2HeartBeat;

int main(void)
{
	IERG3810_SYSTICK_Init1ms();
	IERG3810_clock_tree_Init();
	IERG3810_USART2_Init(36, 9600);
	IERG3810_Buzzer_Init();
	IERG3810_TFTLCD_Init();
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key2_ExtiInit();
	
	while (1)
	{
		if(task1HeartBeat>=200)
		{
			task1HeartBeat=0;
			GPIOB->ODR ^= 1 << 5;
		}
		switch (state)
		{
			case 0:
				name_and_SID();
				break;
			case 1:
				main_menu();
				break;
			case 2:
				launch_game();
				break;
			case 3:
				select_restart();
				break;				
		}
	}
}

void IERG3810_SYSTICK_Init1ms(void)
{
	//systick
	SysTick->CTRL = 0; //clear
	SysTick->LOAD = 72000/8 - 1; // 72 MHz = 72000000; 1 ms then 1000 Hz, and then /8
	// CLKSOURCE = 0: STCLK (FCLK/8)
	// clock tree refers, pend handler
	SysTick->CTRL |= 0x03; // to be modified
	//set internal clk, use interrupt, start count
}

void EXTI2_IRQHandler(void)
{
	state = (state+1)%4;
	
	EXTI->PR = 1 << 2;
}

void name_and_SID(void)
{
	IERG3810_TFTLCD_FillRectangle(0xFFE0, 100, 100, 100, 100);
}

void main_menu(void)
{
	IERG3810_TFTLCD_FillRectangle(0xFF00, 100, 50, 100, 50);
}

void launch_game(void)
{
	IERG3810_TFTLCD_FillRectangle(0xFF00, 100, 150, 100, 150);
}

void select_restart(void)
{
	IERG3810_TFTLCD_FillRectangle(0xFF00, 100, 150, 100, 150);
}
