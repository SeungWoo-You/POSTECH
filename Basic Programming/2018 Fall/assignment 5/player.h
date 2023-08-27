#ifndef PLAYER_H	// 헤더파일 중복 방지 
#define PLAYER_H


typedef struct {
	char name[22];
	char gender;
	char dept[21];
	int stdID;
	int group;
} STUDENT;	// 학생 정보 구조체. 이름은 띄어쓰기+\0를 고려해 22칸, 학과는 \0를 고려해 21칸으로 배열 설정 

#include "group.h"	// 컴파일 순서를 고려할 때 여기에 선언되어야 함 

void P_Show(LIST *slist, int n);	// show 명령 입력시 실행. 목록의 학생 출력 
void P_Add(LIST *slist);	// add 명령 입력시 실행. 학생 1명 추가 
void P_Remove(LIST *slist);	// remove 명령 입력시 실행. 학생 1명 제거 
void P_Play(LIST *slist);	// play명령 입력시 실행. 게임 진행 
void P_Quit(LIST *slist);	// quit명령 입력시 실행. 프로그램 종료 

void line(int a);

#endif
