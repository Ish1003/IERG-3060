#include "stm32f10x.h"
#include "IERG3810_KEY.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_LED.h"


int main(void)
{
	u32 PE3_prev_state = 0;
	u32 PE3_state = 0;
	u32 PA0_prev_state = 0;
	u32 PA0_state = 0;
	u32 count = 400000;
	u32 i = count;
	u32 j = count;
	
  IERG3810_KEY_Init();
	IERG3810_Buzzer_Init();
	IERG3810_LED_Init();
	
  while (1)
  {
		// LED0 and key2
		if((GPIOE->IDR) & 0x00000004)
		{
			GPIOB->ODR |=  1 << 5;
		}
		else
		{
			GPIOB->ODR &= ~ (1<<5);
		}

		//LED1 and key1
		PE3_state = GPIOE->IDR & 0x00000008; //E3
		if( PE3_state ^ PE3_prev_state) //if different
		{
			if (i >= count){
				i = 0;
				GPIOE->ODR ^= 1 << 5;
			}
			PE3_prev_state = PE3_state;
		}
		
		//buzzer and keyup
		PA0_state = GPIOA->IDR & 0x00000001;
		if( PA0_state ^ PA0_prev_state)
		{
			if (j >= count){
				j = 0;
				GPIOB->ODR  ^=  1<<8;
			}
			PA0_prev_state = PA0_state;
		}
		
		i++;
		j++;
  }
}

