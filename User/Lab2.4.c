#include "stm32f10x.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_USART.h"

void Delay(u32 count);

int main(void)
{
  IERG3810_clock_tree_Init();
	IERG3810_USART1_Init(72, 9600);
	Delay(8000000);
  while (1)
  {
		USART_print(1, "Hello, World!");
  }
}

void Delay(u32 count)
{
	u32 i;
	for (i = 0; i < count; i++);
}

