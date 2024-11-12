#ifndef __IERG3810_USART2_H
#define __IERG3810_USART2_H
#include "stm32f10x.h"

void IERG3810_USART2_Init(u32 pclk1, u32 bound);
void IERG3810_USART1_Init(u32 pclk1, u32 bound);
void USART_print(u8 USARTport, char *st);

#endif


