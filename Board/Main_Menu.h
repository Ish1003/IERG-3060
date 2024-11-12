#ifndef __MAIN_MENU_H
#define __MAIN_MENU_H

#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_TFTLCD.h"

void main_menu(void);
void drawTitle(void);
void drawDifficulty(void);
void displayMainMenu(void);
u8 getDifficulty(void);

#endif