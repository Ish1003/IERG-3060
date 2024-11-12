#include "stm32f10x.h"
#include "IERG3810_clock_tree.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_USART.h"
#include "IERG3810_NVIC.h"
#include "IERG3810_LED.h"
#include "IERG3810_Buzzer.h"
#include "IERG3810_Clock.h"

#include "Name_and_SID.h"
#include "Main_Menu.h"

u8 state=0;
u8 task1HeartBeat;
u8 task2HeartBeat;
u8 timerEnable;
u8 timerHeartBeat;

int difficulty = 1;
char all_cards[18] = "123456789ABCDEFGHI";
char cards[36]; //real cards order
char cards_masked[36]; // value indicates existence
int ifend=0;
int end=0;
u8 i, j;
int timepass=0;

void IERG3810_SYSTICK_Init10ms(void);

void launch_game(void);
void select_restart(void);


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

void displayWin(void);
void displayLose(void);
void displayEnd(int end); //1 win 0 lose

int main(void)
{
	IERG3810_SYSTICK_Init10ms();
	IERG3810_clock_tree_Init();
	IERG3810_USART2_Init(36, 9600);
	IERG3810_Buzzer_Init();
	IERG3810_TFTLCD_Init();
	IERG3810_LED_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key2_ExtiInit();
	
	while (1)
	{
		if(task1HeartBeat>=10)
		{
			Delay(5000);
			task1HeartBeat=0;
			switch (state)
			{
				case 0:
					name_and_SID();
					break;
				case 1:
					main_menu();
					break;
				case 2:
					launch_game();
					break;
				case 3:
					select_restart();
					break;
			}
		}
	}
}

void IERG3810_SYSTICK_Init10ms(void)
{
	//systick
	SysTick->CTRL = 0; //clear
	SysTick->LOAD = 720000/8 - 1; // 72 MHz = 72000000; 1 ms then 1000 Hz, and then /8
	// CLKSOURCE = 0: STCLK (FCLK/8)
	// clock tree refers, pend handler
	SysTick->CTRL |= 0x03; // to be modified
	//set internal clk, use interrupt, start count
}

void EXTI2_IRQHandler(void)
{
	state = (state+1)%4;
	Delay(5000);
	IERG3810_TFTLCD_FillRectangle(0x0000, 10, 220, 10, 300);
	Delay(5000);
	EXTI->PR = 1 << 2;
}


void launch_game(void)
{
	// to be changed
	//IERG3810_TFTLCD_FillRectangle(0x070F, 0, 150, 100, 50);
	launchGame(difficulty);
}

void select_restart(void)
{
	// to be changed
	IERG3810_TFTLCD_FillRectangle(0xFFFF, 10, 200, 10, 40);
}


void shuffleCards(char* cards, int len) {
    // Shuffle the array
	;
}

void generate16Cards()
{
    shuffleCards(all_cards, 18);
    for(i=0; i<8; i++)
    {
        cards[i] = all_cards[i];
        cards[i+8] = all_cards[i];
    }
    shuffleCards(cards, 16);
}

void generate24Cards()
{
    shuffleCards(all_cards, 18);
    for(i=0; i<12; i++)
    {
        cards[i] = all_cards[i];
        cards[i+12] = all_cards[i];
    }
    shuffleCards(cards, 24);
}

void generate36Cards()
{
    shuffleCards(all_cards, 18);
    for(i=0; i<18; i++)
    {
        cards[i] = all_cards[i];
        cards[i+18] = all_cards[i];
    }
    shuffleCards(cards, 36);
}

void generateCards(int difficulty)
{
    switch (difficulty)
    {
        case 1:
            generate16Cards();
            break;
        
        case 2:
            generate24Cards();
            break;
        
        case 3:
            generate36Cards();
            break;
        
        default:
            break;
    }
}

void drawTimer(){
    ;
	// this is a task, which refreshes every 50ms or sth. use heartbeatTimer
	IERG3810_TFTLCD_FillRectangle(0x07E0, 10, 220 - timepass, 300, 10);
	if(timepass<220)
	{
		if(timerHeartBeat >= 100)
		{
			timerHeartBeat=0;
			timepass+=2;
			IERG3810_TFTLCD_FillRectangle(0x07E0, 10, 220 - timepass, 300, 10);
			IERG3810_TFTLCD_FillRectangle(0x0000, 230 - timepass, timepass, 300, 10);
		}
	}
}

void drawCards(int difficulty){
    ;
	// to be changed
	// example of 16 cards
	for (i = 0 ; i < 4; i++)
	{
		for (j = 0 ; j < 4; j++)
		{
			//IERG3810_TFTLCD_FillRectangle(0xFF00, 10 + i*40 , 30, 10 + j*50, 40);
			IERG3810_TFTCD_ShowChar(30 + i*40, 50 + j*50, cards[i+j*4], 0x0000, 0x001F);
			//to be replaced with actual functions
		}
	}
}

void flipCard(int x, int y){
    ;
}

void launchGame(int difficulty)
{
    generateCards(difficulty);
		timerEnable = 1;
    drawTimer();
    drawCards(difficulty);
}

void displayWin(){
    end = 1;
    ifend = 1;
}

void displayLose(){
    end = 0;
    ifend = 1;
}

void displayEnd(int end){
    switch (end)
    {
    case 1/* constant-expression */:
        /* code */
        displayWin();
        break;
    
    default:
        displayLose();
        break;
    }
} //1 win 0 lose

