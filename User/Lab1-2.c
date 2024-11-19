#include "stm32f10x.h"

void Delay(u32 count)
{
	u32 i;
	for (i = 0; i < count; i++);
}

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, & GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOE, & GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE, GPIO_Pin_5);
	
  while (1)
  {
		//key up press = high; pull down; initial low
		//key 0 1 2 press = low; pull up; initial high
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) != RESET)
		{
				GPIOB->BRR = 1 << 5;
		}
		else
		{
				GPIOB->BSRR = 1 << 5;
		}
	}
}
