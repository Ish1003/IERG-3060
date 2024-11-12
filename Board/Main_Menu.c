#include "stm32f10x.h"
#include "Main_Menu.h"
#include "IERG3810_TFTLCD.h"

u8 a, b;
u16 color[2] = {0x00FF, 0xFF00};

void main_menu(void)
{
	// to be changed
	displayMainMenu();
}

void drawTitle()
{
	IERG3810_TFTLCD_FillRectangle(0x07FF, 10, 100, 200, 100);
}

void drawDifficulty()
{
	//change_update to show the selected -- with a darker background color
	for(a=0; a<3; a++){
			IERG3810_TFTLCD_FillRectangle(0x00FF, 10, 150, 40 + 20*a, 10);
	}
}

void displayMainMenu()
{
    drawTitle();
    drawDifficulty();
}

u8 getDifficulty(void)
{
	// to be changed
  ;
}