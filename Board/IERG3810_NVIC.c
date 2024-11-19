#include "stm32f10x.h"
#include "IERG3810_NVIC.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"
#include "IERG3810_LED.h"


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

void IERG3810_keyUP_ExtiInit(void)
{
	//keyUP, PA0, exti-0, IRQ 6
	RCC->APB2ENR |= 1 << 2;
	GPIOA->CRL &= 0xFFFFFFF0;
	GPIOA->CRL |= 0x00000008;
	GPIOA->ODR &= ~(0x1) ;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFFFFF0;
	AFIO->EXTICR[0] |= 0x00000000;
	EXTI->IMR |= 1;
	//EXTI->FTSR |= 1;
	EXTI->RTSR |= 1;
	
	NVIC->IP[6] = 0x95;  //priority, 00110101
	NVIC->ISER[0] &= ~(1<<6);
	
	NVIC->ISER[0] |= (1<<6);

}

void IERG3810_key0_ExtiInit(void)
{
	//key0, PE4, exti-4, IRQ 10
	RCC->APB2ENR |= 1 << 6;
	GPIOE->CRL &= 0xFFF0FFFF;
	GPIOE->CRL |= 0x00080000;
	GPIOE->ODR |= 1 << 4;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[1] &= 0xFFFFFFF0;
	AFIO->EXTICR[1] |= 0x00000004;
	EXTI->IMR |= 1 << 4;
	EXTI->FTSR |= 1 << 4;
	//EXTI->RTSR |= 1;
	
	NVIC->IP[10] = 0x75;  //priority, 00110101
	NVIC->ISER[0] &= ~(1<<10);
	
	NVIC->ISER[0] |= (1<<10);

}

void IERG3810_key1_ExtiInit(void)
{
	//key1, PE3, exti-3, IRQ 9
	RCC->APB2ENR |= 1 << 6;
	GPIOE->CRL &= 0xFFFF0FFF;
	GPIOE->CRL |= 0x00008000;
	GPIOE->ODR |= 1 << 3;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFF0FFF;
	AFIO->EXTICR[0] |= 0x00004000;
	EXTI->IMR |= 1 << 3;
	EXTI->FTSR |= 1 << 3;
	//EXTI->RTSR |= 1;
	
	NVIC->IP[9] = 0x95;  //priority, 00110101
	NVIC->ISER[0] &= ~(1<<9);
	
	NVIC->ISER[0] |= (1<<9);

}

void IERG3810_PS2key_ExtiInit(void)
{
	//CLK connects to PC11, DATA connects to PC10, CLK is interrupt, exti 11 #IRQ40
	//PC11 setup,
	RCC->APB2ENR |= 1 << 4;
	GPIOC->CRH &= 0xFFFF0FFF;
  GPIOC->CRH |= 0x00008000;
	GPIOC->ODR |= (1 << 11);
	//PC10 setup
	GPIOC->CRH &= 0xFFFFF0FF;
  GPIOC->CRH |= 0x00000800;
	GPIOC->ODR |= (1 << 10);
	//enable AFIO clk
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[2] &= 0xFFFF0FFF;
	AFIO->EXTICR[2] |= 0x00002000; //0010, PC
	
	EXTI->IMR |= 1 << 11;
	EXTI->FTSR |= 1 << 11;
	
	NVIC->IP[40] = 0x25;
	NVIC->ISER[1] |= (1<<8); //40, ISER1[8]
	
}

void IERG3810_NVIC_SetPriorityGroup(u8 prigroup)
{
	u32 temp, temp1;
	temp1 = prigroup & 0x00000007;
	temp1 <<= 8;
	temp = SCB->AIRCR;
	temp &= 0x0000F8FF;
	temp |= 0x05FA0000;
	temp |= temp1;
	SCB->AIRCR = temp;
}

/*void EXTI2_IRQHandler(void)
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
}*/

/*void EXTI0_IRQHandler(void)
{
	u8 i;
	for(i = 0; i < 10; i++)
	{
		//DS1 on
		GPIOE->BRR |= 1 << 5;
		Delay(1000000);
		//DS1 off
		GPIOE->BSRR |= 1 << 5;
		Delay(1000000);
	};
	EXTI->PR = 1;
}*/

