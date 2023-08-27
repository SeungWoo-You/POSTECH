#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"	// group.h�� player.h ���ο� ���ԵǾ� �����Ƿ� �������� �ʾƵ� �� 

int main(int argc, char* argv[]) {
	char command[10], show[10] = "show", add[10] = "add", remove[10] = "remove", play[10] = "play", quit[10] = "quit";
	int i, sel;
	LIST* slist = (LIST*)malloc(sizeof(LIST));
	
	slist->tail = (NODE**)malloc(sizeof(NODE*) * 5); // �׷� 5�� ����. tail[0]�� �׷� 1, tail[1]�� �׷� 2, ... , tail[4]�� �׷� 5�� linked list�� ����� ���� 
	slist->count = (int*)malloc(sizeof(int) * 5); // �� �׷��� ��� ���� �˷��ִ� count ����. ���� ���������� count[0]�� �׷� 1, count[1]�� �׷�2, ... �� ������ �����ϴ� ��� 
	
	for(i = 0; i < 5; i++) { // linked list �ʱ�ȭ 
		slist->tail[i] = NULL;
		slist->count[i] = 0;
	}
	
	G_LoadData(slist, argv); 
	
	printf("Name\t\t\tGender\tDept\t\t\tStudentID\n\n"); // ������ ���߱� ���� \t������ ��� 
	for(i = 0; i < 5; i++) {
		P_Show(slist, i);
	}
	printf("\n");
	printf(">> ");
	
	while(1) {
		scanf("%s", command);
		if(stricmp(command, show) == 0) {	// ��ҹ��ڸ� �����ϰ� �� ���ڿ��� ���ϴ� �Լ� stricmp��� 
			printf("Which group? (0 : all) ");
			
			while(1) {
				scanf("%d", &sel);
				if(sel > 5 || sel <= 0) {
					if(sel == 0) {	// 0�� �Է¹����� ���� ��� 
						printf("Name\t\t\tGender\tDept\t\t\tStudentID\n\n");
						for(i = 0; i < 5; i++) {
							P_Show(slist, i);
						}
					}
					else continue;	// �ùٸ� ����(0 ~ 5)�� �Է¹��� �� ���� �ݺ� 
				}
				break;
			}
			
			if(sel != 0) {	// 0�� �ƴϸ� ������ ��ȣ�� group�� �������� �Ѵ� 
				printf("Name\t\t\tGender\tDept\t\t\tStudentID\n\n");
				P_Show(slist, sel - 1);	// sel - 1�� ������ ������ ������ ���� ����. tail[1]�� group 1�� �ƴ϶� group 2�� ��Ÿ���� 
			}
			printf("\n");
			printf(">> ");
			
			continue;
		}
		if(stricmp(command, add) == 0) {
			P_Add(slist);
			printf("\n");
			printf(">> ");
			continue;
		}
		if(stricmp(command, remove) == 0) {
			P_Remove(slist);
			printf("\n");
			printf(">> ");
			continue;
		}
		if(stricmp(command, play) == 0) {
			P_Play(slist);
			printf("\n");
			printf(">> ");
			continue;
		}
		if(stricmp(command, quit) == 0) {
			P_Quit(slist);
		}
		
		printf(">> ");	// �ùٸ� ���ڿ��� �Է¹��� ������ ��� ����ȴ� 
	}
	
	return 0;
}
