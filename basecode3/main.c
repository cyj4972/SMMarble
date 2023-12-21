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
		printf("%s 졸업을 축하합니다!!\n", cur_player[player].name);
		printf("----------------GAME OVER----------------");
		
		return cur_player[player].flag_graduate;
	 } 
 } 
#if 0
//강의 수강 여부 확인 
int takeLecture(int player, char *lectureName)
{
	char c;
	smmObjGrade_e lectureGrade;
	//플레이어가 강의 노드에 위치
	if(smmObj_getNodeType(cur_player[player].position) == SMMNODE_TYPE_LECTURE)
	{
		//수강 여부 확인 
		printf("%s를 수강하시겠습니까?(Y/N)\n", lectureName);
		c = getchar;
		fflush(stdin);
		
		//수강할 경우
		if(c = 'Y')
		{
			
		 } 
	 } 
	
 } 

//성적 출력
void printGrades(int player)
{
	int i;
	void *gradePtr;
	for(i=0;i<smmdb_len(LISTNO_OFFSET_GRADE + player);i++)
	{
		gradePtr = smmdb_getData(LISTNO_OFFSET_GRADE + player, i)
		if()
		
	}
 } 
#end if

//player 정보 출력 
void printPlayerStatus(void)
{
	int i;
	//모든 플레이어의 정보가 출력될 때까지 반복
	for(i=0;i<player_nr;i++)
	{
		//플레이어 정보
		printf("%s : credit %i, energy %i, position %i\n",
					cur_player[i].name,
					cur_player[i].accumCredit,
					cur_player[i].energy.
					cur_player[i].position);
	 } 
 }
 
 //player 생성 
void generatePlayers(int n, int initEnergy)
{
	int i;
	//n time loop
	for(i=0;i<n;i++)
	{
		//input name
		printf("Input player %i's name : ", i);
		scanf("%s", cur_player[i].name);
		fflush(stdin);
		
		//set position
		cur_player[i].position = 0;
		//set energy
		cur_player[i].energy = initEnergy;
		//set credit
		cur_player[i].accumCredit = 0;
		//재학 중인 상태 
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
	void *boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
    int type = smmObj_getNodeType( boardPtr );
    char *name = smmObj_getNodeName( boardPtr );
    void *gradePtr;
	int lectureGrade; 
	
    switch(type)
    {
        //case lecture:
        case SMMNODE_TYPE_LECTURE:
            if(takeLecture(player, smmObj_getNodeName(gradePtr)) == 'Y')
            cur_player[player].accumCredit += smmObj_getNodeCredit( boardPtr );
            cur_player[player].energy -= smmObj_getNodeEnergy( boardPtr );
            
            //grade generation
            gradePtr = smmObj_genObject(name, smmObjType_grade, 0, smmObj_getNodeCredit( boardPtr ), 0, lectureGrade);
            smmdb_addTail(LISTNO_OFFSET_GRADE + player, gradePtr);
            
            break;
            
        default:
            break;
    }
}

void goForward(int player, int step)
{
    void *boardPtr;
    cur_player[player].position += step;
    boardPtr = smmdb_getData(LISTNO_NODE, cur_player[player].position );
     
    printf("%s go to node %i (name: %s)\n", 
                cur_player[player].name, cur_player[player].position,
                smmObj_getNodeName(boardPtr);
}

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
	while (fscanf(fp, "%s %i %i %i", name, &type, &credit, &energy) == 4)	//read a node parameter set
    {
        //store the parameter set
        void *boardObj = smmObj_genObject(name, smmObjType_board, type, credit, energy, 0);
        smmdb_addTail(LISTNO_NODE, boardObj);
        
        if (type == SMMNODE_TYPE_HOME)
           energy = initEnergy;
        board_nr++;
    }
    fclose(fp);
    printf("Total number of board nodes : %i\n", board_nr);
    
    for (i = 0;i<board_nr;i++)
    {
        void *boardObj = smmdb_getData(LISTNO_NODE, i);
        
        printf("node %i : %s, %i(%s), credit %i, energy %i\n", 
                     i, smmObj_getNodeName(boardObj), 
                     smmObj_getNodeType(boardObj), smmObj_getTypeName(smmObj_getNodeType(boardObj)),
                     smmObj_getNodeCredit(boardObj), smmObj_getNodeEnergy(boardObj));
    }
    
    //2. food card config 
    if ((fp = fopen(FOODFILEPATH,"r")) == NULL)
    {
        printf("[ERROR] failed to open %s. This file should be in the same directory of SMMarble.exe.\n", FOODFILEPATH);
        return -1;
    }
    
    printf("\n\nReading food card component......\n");
    while (fscanf(fp, "%s %i", name, &energy) == 2) //read a food parameter set
    {
        //store the parameter set
        void *foodObj = smmObj_genObject(name, smmObjType_card, 0, 0, energy, 0);
        smmdb_addTail(LISTNO_FOODCARD, foodObj);
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
    while (fscanf(fp,"%s", name) == 1)	//read a festival card string
    {
        //store the parameter set
        void *festObj = smmObj_genObject(name, smmObjType_card, 0, 0, 0, 0);
        smmdb_addTail(LISTNO_FESTCARD, festObj);
    }
    fclose(fp);
    printf("Total number of festival cards : %i\n", festival_nr);
    
    
    
    //2. Player configuration ---------------------------------------------------------------------------------
 	do
    {
        //input player number to player_nr
        printf("input player no.:");
        scanf("%d", &player_nr);
        fflush(stdin);
    }
    while (player_nr < 0 || player_nr >  MAX_PLAYER);
    
    cur_player = (player_t*)malloc(player_nr*sizeof(player_t));
    generatePlayers(player_nr, initEnergy);
    
    
    //3. SM Marble game starts ---------------------------------------------------------------------------------
	printf("======================\n");
	printf("SMMarble\n");
	printf("======================\n");
	
	//졸업 요건 여부에 따라 반복  
    while (cur_player[turn].flag_graduate != 1) //is anybody graduated?
    {
        int die_result;
        
        //4-1. initial printing
        printPlayerStatus();
        
        //4-2. die rolling (if not in experiment)
        die result = rolldie(turn);
        
        //4-3. go forward
        goForward(turn, die_result);

		//4-4. take action at the destination node of the board
        actionNode(turn);
        
        //4-5. next turn
        turn = (turn + 1)%player_nr;
        
        //4-6. 졸업 여부 확인 
        isGraduated(turn); 
    }
    
    free(cur_player);
    
    system("PAUSE");
    return 0;
}
