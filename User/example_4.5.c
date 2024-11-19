#include "stm32f10x.h"
#include "IERG3810_LED.h"
#include "IERG3810_Clock.h"
#include "IERG3810_TFTLCD.h"

void Delay(u32 count) 
{
	u32 i;
	for(i = 0; i < count; i++);
}


void IERG3810_key2_ExtiInit(void)
{
	//KEY2 AT PE2, EXTI-2,IRQ#8
	RCC->APB2ENR |= 1 << 6; 
	GPIOE->CRL &= 0XFFFFF0FF;
	GPIOE->CRL |= 0X00000800;
	GPIOE->ODR |= 1 << 2;
	RCC->APB2ENR |= 0X01;	
	AFIO->EXTICR[0] &= 0XFFFFF0FF;	
	AFIO->EXTICR[0] |= 0X00000400;	
	EXTI->IMR |= 1 << 2;	
	EXTI->FTSR |= 1 << 2;	
	
	NVIC->IP[8] = 0X65;	
	NVIC->ISER[0] |= 1 << 8;	
}

void IERG3810_keyUP_ExtiInit(void)
{
	//KEYUP AT PA0, EXTI-0,IRQ#6
	RCC->APB2ENR |= 1 << 2; 
	GPIOA->CRL &= 0XFFFFFFF0;
	GPIOA->CRL |= 0X00000008;
	GPIOA->ODR |= 0 << 0;
	RCC->APB2ENR |= 0X01;	
	
	AFIO->EXTICR[0] &= 0XFFFFFFF0;	
	AFIO->EXTICR[0] |= 0X00000000;	
	EXTI->IMR |= 1 << 0;	
	EXTI->FTSR |= 1 << 0;	
	
	NVIC->IP[6] = 0X35;	 
	NVIC->ISER[0] |= 1 << 6;	
}

void IERG3810_PS2key_ExtiInit(void)
{
	//PS2 AT PC11, EXTI-11, IRQ#40
	RCC->APB2ENR |= 1 << 4; 
	GPIOC->CRH &= 0XFFFFF0FF;
	GPIOC->CRH |= 0X00000800;
	GPIOC->ODR |= 1 << 2;
	RCC->APB2ENR |= 0X01;	
	AFIO->EXTICR[2] &= 0XFFFF0FFF;	
	AFIO->EXTICR[2] |= 0X00002000;	
	EXTI->IMR |= 1 << 11;	
	EXTI->FTSR |= 1 << 11;	
	
	// for each EXTI, values for IP and ISER are different
	NVIC->IP[40] = 0X95;	
	NVIC->ISER[1] |= 1 << 8;	
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
	for (i = 0; i < 5; i++) {
		DS0_ON;
		Delay(50000000);
		DS0_OFF;
		Delay(50000000);
	}
	EXTI->PR = 1 << 2;	//clear this exception pending bit
}

void EXTI0_IRQHandler(void)
{
	u8 i;
	for (i = 0; i < 5; i++) {
		DS1_ON;
		Delay(50000000);
		DS1_OFF;
		Delay(50000000);
	}
	EXTI->PR = 1 << 0;
}

u32 ps2count = 0;
u32 timeout = 20000;
u32 ps2key = 0;

void EXTI15_10_IRQHandler(void)
{
	u16 bit = (GPIOC->IDR & (1 << 10)) >> 10;
	if (ps2count > 0 && ps2count < 9) {
		ps2key |= bit << (ps2count - 1);
	}
	ps2count++;
	Delay(10);
	EXTI->PR = 1 << 11;
}

void Exp0401()
{
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key2_ExtiInit();
	DS0_OFF;
	while (1) 
	{
		Delay(5000000);
		DS1_ON;
		Delay(5000000);
		DS1_OFF;
	}
}

void Exp0402()
{
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key2_ExtiInit();
	IERG3810_keyUP_ExtiInit();
	DS0_OFF;
	DS1_OFF;
	while (1) 
	{	
	}
}


void Exp0405()
{
	u8 state = 0;
	IERG3810_clock_tree_init();
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key2_ExtiInit();
	IERG3810_keyUP_ExtiInit();
	IERG3810_PS2key_ExtiInit();
	DS0_OFF;
	DS1_OFF;
	Delay(10000000);
	while (1) 
		{
		if (ps2count >= 11) 
			{
			if (ps2key == 0x6B || ps2key == 0x73) 
				{
				state = 1;
				} 
			else 
				{
				if (!state) 
				{
				DS1_ON;
				}	
				else 
				{
				state = 0;
				DS1_OFF;
				}
			}						
			ps2count = 0;
			ps2key = 0;
			EXTI->PR = 1 << 11;
		}
		timeout--;
		
		if (timeout == 0) {
			timeout = 20000;
			ps2key = 0;
			ps2count = 0;
		}
	}
}

int main()
{
	Exp0405();
}
