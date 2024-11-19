#ifndef __LAUNCH_GAME_H
#define __LAUNCH_GAME_H

#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"

void launch_game(void);

void generateCards(int difficulty);
void countDown(void);
void shuffleCards(char* cards, int len);
void generate16Cards(void);
void generate24Cards(void);
void generate36Cards(void);
void drawTimer(void);
void drawCards(int difficulty);
void flipCard(int x, int y);
void launchGame(int difficulty);

#endif