#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"	// group.h는 player.h 내부에 포함되어 있으므로 포함하지 않아도 됨 

int main(int argc, char* argv[]) {
	char command[10], show[10] = "show", add[10] = "add", remove[10] = "remove", play[10] = "play", quit[10] = "quit";
	int i, sel;
	LIST* slist = (LIST*)malloc(sizeof(LIST));
	
	slist->tail = (NODE**)malloc(sizeof(NODE*) * 5); // 그룹 5개 설정. tail[0]은 그룹 1, tail[1]은 그룹 2, ... , tail[4]는 그룹 5의 linked list를 만드는 역할 
	slist->count = (int*)malloc(sizeof(int) * 5); // 각 그룹의 멤버 수를 알려주는 count 설정. 위와 마찬가지로 count[0]은 그룹 1, count[1]은 그룹2, ... 의 정보를 저장하는 방식 
	
	for(i = 0; i < 5; i++) { // linked list 초기화 
		slist->tail[i] = NULL;
		slist->count[i] = 0;
	}
	
	G_LoadData(slist, argv); 
	
	printf("Name\t\t\tGender\tDept\t\t\tStudentID\n\n"); // 간격을 맞추기 위해 \t여러번 사용 
	for(i = 0; i < 5; i++) {
		P_Show(slist, i);
	}
	printf("\n");
	printf(">> ");
	
	while(1) {
		scanf("%s", command);
		if(stricmp(command, show) == 0) {	// 대소문자를 무시하고 두 문자열을 비교하는 함수 stricmp사용 
			printf("Which group? (0 : all) ");
			
			while(1) {
				scanf("%d", &sel);
				if(sel > 5 || sel <= 0) {
					if(sel == 0) {	// 0을 입력받으면 전부 출력 
						printf("Name\t\t\tGender\tDept\t\t\tStudentID\n\n");
						for(i = 0; i < 5; i++) {
							P_Show(slist, i);
						}
					}
					else continue;	// 올바른 숫자(0 ~ 5)를 입력받을 때 까지 반복 
				}
				break;
			}
			
			if(sel != 0) {	// 0이 아니면 선택한 번호의 group을 출력해줘야 한다 
				printf("Name\t\t\tGender\tDept\t\t\tStudentID\n\n");
				P_Show(slist, sel - 1);	// sel - 1인 이유는 위에서 설명한 구조 때문. tail[1]이 group 1이 아니라 group 2를 나타낸다 
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
		
		printf(">> ");	// 올바른 문자열을 입력받지 못했을 경우 실행된다 
	}
	
	return 0;
}
