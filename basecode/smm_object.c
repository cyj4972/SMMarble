//
//  smm_node.c
//  SMMarble
//
//  Created by Juyeop Kim on 2023/11/05.
//

#include "smm_common.h"
#include "smm_object.h"
#include <string.h>
 
static char smmNodeName[SMMNODE_TYPE_MAX][MAX_CHARNAME] = {
	"강의",
	"식당",
	"실험실",
	"집",
	"실험실로 이동",
	"음식찬스",
	"축제"
};

char *smmObj_getTypeName(int type)
{
	return (char*)smmNodeName[type];
} 

//1. 구조체 형식 정의
typedef struct smmObject{
	char name[MAX_CHARNAME];
	smmObjType_e objType;
	int type;
	int credit;
	int energy;
	smmObjGrade_e grade;
}smmObject_t;

//2. 구조체 배열 변수 선언 
static smmObject_t smm_node[MAX_NODE];
static int smmObj_noNode=0;


//3. 기존 함수 수정 및 새로 작성 
//object generation
int smmObj_genObject(char* name, smmObjType_e objType, int type, int credit, int energy, smmObjGrade_e grade)
{ 
	smmObject_t* ptr;
	
	ptr = (smmObject_t*)malloc(sizeof(smmObject_t));
	
	strcpy(ptr->name, name);
	ptr->objType = objType;
	ptr->type = type;
	ptr->credit = credit;
	ptr->energy = energy;
	ptr->grade = grade;
	
	return ptr;
}

//member retrieving
char* smmObj_getNodeName(void* obj)
{	
	smmObject_t* ptr = (smmObject_t*)obj;
	 
    return ptr->name;
}

int smmObj_getNodeType(int node_nr)
{
    return smm_node[node_nr].type;
}

int smmObj_getNodeCredit(int node_nr)
{
	return smm_node[node_nr].credit;
}

int smmObj_getNodeEnergy(int node_nr)
{
	return smm_node[node_nr].energy;
}
