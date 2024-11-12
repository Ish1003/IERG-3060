#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//use case to switch to states

int difficulty = 0;
char all_cards[18] = "123456789ABCDEFGHI";
int state;
int ifend;
int end;

char cards[36]; //real cards order
char cards_masked[36]; // 1/0, 1: '*', 0, ' '

void drawSids();
void drawDifficulty();
void displayMainMenu();
void getDifficulty();

void generateCards(int difficulty);
void countDown();
void shuffleCards(char cards, int len);
void generate16Cards();
void generate24Cards();
void generate36Cards();
void drawTimer();
void drawCards(int difficulty);
void flipCard(int x, int y);
void launchGame(int difficulty);
void displayWin();
void displayLose();
void displayEnd(int end); //1 win 0 lose
void selectRestart();


int main(){
    drawSids(); //wait for tigger
    displayMainMenu(); //wait for trigger
    getDifficulty(); //exception actually
    launchGame(difficulty);

}

void drawIcon()
{
    printf("Matching Game!!");
}

void drawDifficulty()
{
    printf("easy - 1");
    printf("medium - 2");
    printf("hard - 3");
}

void displayMainMenu()
{
    drawIcon();
    drawDifficulty();
}

void getDifficulty()
{
    scanf("%d", &difficulty);
}

void shuffleCards(char cards, int len) {
    // Shuffle the array
    for (int i = 0; i < len; i++) {
        int j = rand() % len;
        char temp = cards[i];
        cards[i] = cards[j];
        cards[j] = temp;
    }
}

void generate16Cards()
{
    shuffleCards(all_cards, 18);
    for(int i=0; i<8; i++)
    {
        cards[i] = all_cards[i];
        cards[i+8] = all_cards[i];
    }
    shuffleCards(cards_easy, 16);
}

void generate24Cards()
{
    shuffleCards(all_cards, 18);
    for(int i=0; i<12; i++)
    {
        cards[i] = all_cards[i];
        cards[i+12] = all_cards[i];
    }
    shuffleCards(cards, 24);
}

void generate36Cards()
{
    shuffleCards(all_cards, 18);
    for(int i=0; i<18; i++)
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
        case 1/* constant-expression */:
            /* code */
            generate16Cards();
            break;
        
        case 2/* constant-expression */:
            /* code */
            generate24Cards();
            break;
        
        case 3/* constant-expression */:
            /* code */
            generate36Cards();
            break;
        
        default:
            break;
    }
}

void drawTimer(){
    ;
}

void drawCards(int difficulty){
    ;
}

void flipCard(int x, int y){
    ;
}

void launchGame(int difficulty)
{
    generateCards(difficulty);
    drawTimer();
    drawCards(difficulty);
    //if triggered event select cards
    if(ifend){
        displayEnd(end);
    }
    selectRestart();
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
        displayWin;
        break;
    
    default:
        displayLose;
        break;
    }
} //1 win 0 lose

void selectRestart(){
    ;
}
