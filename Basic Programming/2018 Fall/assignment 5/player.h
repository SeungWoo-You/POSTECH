#ifndef PLAYER_H	// ������� �ߺ� ���� 
#define PLAYER_H


typedef struct {
	char name[22];
	char gender;
	char dept[21];
	int stdID;
	int group;
} STUDENT;	// �л� ���� ����ü. �̸��� ����+\0�� ����� 22ĭ, �а��� \0�� ����� 21ĭ���� �迭 ���� 

#include "group.h"	// ������ ������ ����� �� ���⿡ ����Ǿ�� �� 

void P_Show(LIST *slist, int n);	// show ��� �Է½� ����. ����� �л� ��� 
void P_Add(LIST *slist);	// add ��� �Է½� ����. �л� 1�� �߰� 
void P_Remove(LIST *slist);	// remove ��� �Է½� ����. �л� 1�� ���� 
void P_Play(LIST *slist);	// play��� �Է½� ����. ���� ���� 
void P_Quit(LIST *slist);	// quit��� �Է½� ����. ���α׷� ���� 

void line(int a);

#endif
