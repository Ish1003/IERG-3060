#include "stm32f10x.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"
#include "IERG3810_LED.h"

void IERG3810_key2_ExtiInit(void);
void IERG3810_NVIC_SetPriorityGroup(u8 prigroup);
void EXTI2_IRQHandler(void);
void IERG3810_keyUP_ExtiInit(void);
void EXTI0_IRQHandler(void);
void IERG3810_PS2key_ExtiInit(void);
void EXTI15_10_IRQHandler(void);

u32 sheep = 0;
u32 timeout = 0;
u32 ps2count = 0;
//store keyboard data
u32 ps2key = 0;
u8 ps2dataReady = 0;

u32 ps2key1 = 0x70;
u32 ps2key_release = 0xF0;
u32 ps2key2 = 0x72;

	
int main(void)
{
	IERG3810_clock_tree_Init();
	IERG3810_USART2_Init(36, 9600);
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key2_ExtiInit();
	IERG3810_keyUP_ExtiInit();
	IERG3810_PS2key_ExtiInit();
	//USART_print(2, "1234567890");
	//DS0 off
	GPIOB->BSRR |= 1 << 5;
	//DS1 off
	GPIOE->BSRR |= 1 << 5;
	while(1)
	{
		//USART_print(2, " --- ABCDEF ");
		sheep++;
		if(ps2count >= 11) // if keyboard recieved data correctly
		{
			EXTI->IMR &= ~(1 << 11);
			//student designed program
			//process scancode, ps2key now store 8 bit of data
			if(ps2key == ps2key1 || ps2key == ps2key2 )
			{
				//DS0 on
				GPIOB->BRR |= 1 << 5;
				Delay(10000000);
				//DS0 off
				GPIOB->BSRR |= 1 << 5;
				Delay(10000000);
			}
			ps2key = 0;
			ps2count = 0;
			EXTI->PR = 1 << 11;
			EXTI->IMR |= (1<<11);
		}
		timeout --;
		
		if(timeout == 0)
		{
			timeout = 20000;
			ps2key = 0;
			ps2count = 0;
		}
	}
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
	
	NVIC->IP[6] = 0x35;  //priority, 00110101
	NVIC->ISER[0] &= ~(1<<6);
	
	NVIC->ISER[0] |= (1<<6);

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

void EXTI0_IRQHandler(void)
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
}

void EXTI15_10_IRQHandler(void) //EXTI15-10 interrupt
{
	//don't process the scancode
	//student designed program
	//jot down databit at each interrupt, store the data into ps2key
	if(ps2count > 0 && ps2count < 9)
	{
		//transmit is little 
		ps2key |= ((GPIOC->IDR & (1 << 10)) >> 10)<<(ps2count - 1);
	};
	ps2count++;
	
	Delay(10);
	EXTI->PR = 1 << 11;
}
