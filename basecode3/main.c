//
//  main.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include <time.h>
#include <string.h>
#include "smm_object.h"
#include "smm_database.h"
#include "smm_common.h"

#define BOARDFILEPATH "marbleBoardConfig.txt"
#define FOODFILEPATH "marbleFoodConfig.txt"
#define FESTFILEPATH "marbleFestivalConfig.txt"


//board configuration parameters
static int board_nr;
static int food_nr;
static int festival_nr;

typedef struct player{
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;
	int flag_graduate;
}player_t;

static player_t *cur_player;

//function prototypes
int isGraduated(int player); //check if any player is graduated
void generatePlayers(int n, int initEnergy); //generate a new player
void printGrades(int player); //print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player
smmGrade_e takeLecture(int player, char *lectureName); //take the lecture (insert a grade of the player)
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history
void printGrades(int player); //print all the grade history of the player

//졸업 여부 확인
int isGraduated(int player)
{
	//플레이어의 학점이 졸업 학점 이상, 플레이어가 집에 도착을 모두 만족해야 졸업
	if(cur_player[player].accumCredit >= GRADUATE_CREDIT && cur_player[player].position >= SMMNODE_TYPE_HOME)
	{
		cur_player[player].flag_graduate = 1;
		
	 } 
 } 



int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    
#if 0
    if (c == 'g')
        printGrades(player);
#endif
    
    return (rand()%MAX_DIE + 1);
}


//action code when a player stays at a node
void actionNode(int player)
{
    switch(type)
    {
        //case lecture:
        default:
            break;
    }
}



int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type;
    int credit;
    int energy;
    
    board_nr = 0;
    food_nr = 0;
    festival_nr = 0;
    
    srand(time(NULL));
    
    
    //1. import parameters ---------------------------------------------------------------------------------
    //1-1. boardConfig 
    if ((fp = fopen(BOARDFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", BOARDFILEPATH);
        getchar();
        return -1;
    }
    
    printf("Reading board component......\n");
    while () //read a node parameter set
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while () //read a food parameter set
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of food cards : %i\n", food_nr);
    
    
    
    //3. festival card config 
    if ((fp = fopen(FESTFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FESTFILEPATH);
        return -1;
    }
    
    printf("\n\nReading festival card component......\n");
    while () //read a festival card string
    {
        //store the parameter set
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    /*
    do
    {
        //input player number to player_nr
    }
    while ();
    generatePlayers();
    */
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while () //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        //printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        
        
        //4-3. go forward
        //goForward();

		//4-4. take action at the destination node of the board
        //actionNode();
        
        //4-5. next turn
        
    }
    
    return 0;
}
