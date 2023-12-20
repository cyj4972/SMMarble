//
//  smm_object.h
//  SMMarble object
//
//  Created by Juyeop Kim on 2023/11/05.
//

#ifndef smm_object_h
#define smm_object_h

//node type 
#define SMMNODE_TYPE_LECTURE		0		//강의 
#define SMMNODE_TYPE_RESTAURANT		1		//식당
#define SMMNODE_TYPE_LABORATORY		2		//실험실 
#define SMMNODE_TYPE_HOME			3		//집 
#define SMMNODE_TYPE_GOTOLAB		4		//실험실로 이동 
#define SMMNODE_TYPE_FOODCHANCE		5		//식사 
#define SMMNODE_TYPE_FESTIVAL		6		//축제 

#define SMMNODE_TYPE_MAX			7

typedef enum smmObjType{
	smmObjType_board = 0,
	smmObjType_card,
	smmObjType_grade
}smmObjType_e;

//성적 
typedef enum smmObjGrade{
	smmObjType_AP = 0,
	smmObjType_A0,
	smmObjType_AM,
	smmObjType_BP,
	smmObjType_B0,
	smmObjType_BM,
	smmObjType_CP,
	smmObjType_C0,
	smmObjType_CM
}smmObjGrade_e;

//object generation
void smmObj_genObject(char* name, smmObjType_e objType, int credit, int energy, smmObjGrade_e grade);

//member retrieving
char* smmObj_getNodeName(int node_nr);
int smmObj_getNodeType(int node_nr);
int smmObj_getNodeCredit(int node_nr);
int smmObj_getNodeEnergy(int node_nr);

//element to string
char* smmObj_getTypeName(int type);

#endif /* smm_object_h */
