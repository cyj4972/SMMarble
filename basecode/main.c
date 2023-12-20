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

static int player_nr;

typedef struct player{
	int energy;
	int position;
	char name[MAX_CHARNAME];
	int accumCredit;
	int flag_graduate;
}player_t;

static player_t *cur_player;

//function prototypes
int isGraduated(void); //check if any player is graduated
	//print grade history of the player
void goForward(int player, int step); //make player go "step" steps on the board (check if player is graduated)
void printPlayerStatus(void); //print all player status at the beginning of each turn
float calcAverageGrade(int player); //calculate average grade of the player		//짜야함
int takeLecture(int player, char *lectureName, int lectureCredit); //take the lecture
void* findGrade(int player, char *lectureName); //find the grade from the player's grade history		//짜야함
void printGrades(int player); //print all the grade history of the player

//졸업 여부 확인 
int isGraduated(void)
{
	int i;
	for (i=0;i<player_nr;i++)
	{
		if(cur_player[i].accumCredit >= GRADUATE_CREDIT)
			cur_player[i].flag_graduate = 1;
			printf("%s 졸업을 축하합니다!!\n", cur_player[i].name);
			break;
	}
	return cur_player[i].flag_graduate = 1;
}

//강의 수강 
int takeLecture(int player, char *lectureName, int lectureCredit)
{
	char c;
	if(smmObj_getNodeType(cur_player[player].position) == SMMNODE_TYPE_LECTURE)
		if(cur_player[player].accumCredit > lectureCredit)
			{
				printf("%s를 수강하시겠습니까?(Y/N)\n", lectureName);
				c = getchar();
    			fflush(stdin);
    		}
    return c;
}

//성적 출력 
void printGrades(int player)
{
	int i;
	void *gradePtr;
	for(i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i);
		if(takeLecture(player, smmObj_getNodeName(gradePtr), smmObj_getNodeCredit(gradePtr)) == 'Y')
			smmObjGrade smmObjGrade_e lectureGrade = rand()%MAX_GRADE;
			printf("%s(수강) : %s\n", smmObj_getNodeName(gradePtr), lectureGrade);
		else
			printf("%s(미수강)\n", smmObj_getNodeName(gradePtr));
	}
} 

//player 정보 출력 
void printPlayerStatus(void)
{
	int i;
	//모든 플레이어의 정보가 출력될 때까지 반복 
	for (i=0;i<player_nr;i++)
	{
		printf("%s : credit %i, energy %i, position %i\n",
					 cur_player[i].name,
					 cur_player[i].accumCredit,
					 cur_player[i].energy,
					 cur_player[i].position);
	}
}

//player 생성 
void generatePlayers(int n, int initEnergy)
{
	int i;
	//n time loop
	for (i=0;i<n;i++)
	{
		//input name
		printf("Input player %i's name : ", i);
		scanf("%s", cur_player[i].name);
		fflush(stdin);
		
		//set position
		cur_player[i].position = 0;
		
		//set energy
		cur_player[i].energy = initEnergy;
		cur_player[i].accumCredit = 0;
		cur_player[i].flag_graduate = 0;
		
	}
}

//주사위 굴리기 
int rolldie(int player)
{
    char c;
    printf(" Press any key to roll a die (press g to see grade): ");
    c = getchar();
    fflush(stdin);
    

    if (c == 'g')
        printGrades(player);
    
    return (rand()%MAX_DIE + 1);
}

//action code when a player stays at a node
void actionNode(int player)
{	
	void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
	int type = smmObj_getNodeType(boardPtr);
	char *name = smmObj_getNodename(boardPtr);
	void *gradePtr;
	
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
        	if(takeLecture(player, smmObj_getNodeName(gradePtr), smmObj_getNodeCredit(gradePtr)) == 'Y')
        	{
        		cur_player[player].accumCredit += smmObj_getNodeCredit(boardPtr);
        		cur_player[player].energy -= smmObj_getNodeEnergy(boardPtr);
				cur_player[player].position += step;
			}
			
			gradePtr= smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit(boardPtr), 0, //성적 rand%()MAX_GRADE);
        	smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
			
			break;
			 
		default:
        break;
    }
}

void goFoward(int player, int step)
{
	void *boardPtr;
	cur_player[player].position += step;
	boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position);
	pritnf("%s go to node %i (name : %s)\n",
				cur_player[player].name, cur_player[player].position,
				smmObj_getNodeName(boardPtr));
}

//게임 실행 과정 
int main(int argc, const char * argv[]) {
    
    FILE* fp;
    char name[MAX_CHARNAME];
    int type; 
    int credit;
    int energy;
    int i;
    int initEnergy;
    int turn = 0;
    
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
    //read a node parameter set
    while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4)
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        
		if(type == SMMNODE_TYPE_HOME)
			initEnergy = energy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
	for (i=0;i<board_nr;i++)
    {
    	void *boardObj = smmdb_getData(LISTNO_NODE, i);
    	
    	printf("node %i : %s, %i(%s), credit %i, energy %i\n",
						i, smmObj_getNodeName(boardObj),
						smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
						smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
	} 
	
    //printf("(%s)", smmObj_getTypeName(SMMNODE_TYPE_LECTURE));
    
    #if 0
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    //read a food parameter set
    while (fscanf(fp, "%s %i", name, &energy) == 2)
    {
        //store the parameter set
        void *foodObj = smmObj_genObject(name, smmObjType_food, type, 0, energy, 0);
        smmdb_addTail(LISTNO_FOODCARD, foodObj);
        
        

        //void *foodObj = smm_getData(LISTNO_FOODCARD, i)
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
    #end if
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
    
    do
    {
        //input player number to player_nr
        printf("input player no. : ");
    	scanf("%d", &player_nr);
    	fflush(stdin);
    }
    while (player_nr < 0 || player > MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player-t));
    generatePlayers(player_nr, initEnergy);
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
    while () //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        die_result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn+1)%player_nr; 
        
    }
    #endif
    free(cur_player);
    
    system("PAUSE");
    return 0;
}
