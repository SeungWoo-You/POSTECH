#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

void P_Show(LIST *slist, int n) { 
	int i;
	NODE *temp;
	
	line(28);
	printf(" GROUP %d ", n + 1);	// n을 배열에 들어갈 숫자(0 ~ 4)로 받기 때문에 원하는 그룹 숫자를 출력하고 싶으면 n + 1을 해줘야 한다 
	line(28); printf("\n");
	
	temp = slist->tail[n];	// temp를 선택한 group의 tail로 이동 
	for(i = 0; i < (slist->count[n]); i++) {
		temp = temp->next;	// temp가 선택한 group의 head부터 시작해 tail에서 끝
		printf("\n%-21s\t%c\t%-20s\t%d\n",temp->data.name, temp->data.gender, temp->data.dept, temp->data.stdID);
		
	}
	printf("\n");
}

void P_Add(LIST *slist) {
	NODE newstd, *temp;	// newstd는 단순 정보 저장, *temp는 linked list 제어용 
	int i, j;
	
	printf("StudentID : ");
	scanf("%d", &(newstd.data.stdID));
	
	
	for(i = 0; i < 5; i++) {	// 해당 학번을 가진 학생이 있는지 검사 
		temp = slist->tail[i]->next;	// 각 group의 head 부터 검사 
		for(j = 0; j < (slist->count[i]); j++) {	// 해당 group의 학생 수 만큼 검사 
			if((temp->data.stdID) == (newstd.data.stdID)) {	// 학생이 존재할 때 
				printf("Player already exists!\n");
				printf("name\t\t\tGender\tdept\t\t\tStudentID\tGroup\n");
				printf("%-21s\t%c\t%-20s\t%d\t%d\n",temp->data.name, temp->data.gender, temp->data.dept, temp->data.stdID, temp->data.group);	// 왼쪽 정렬, 자릿수 조정 
				return;
			}
			else
			temp = temp->next;
		}
	}
	
	printf("Name : ");
	while (getchar() != '\n');	// 버퍼 초기화. 버퍼 때문에 에러가 발생하는 것을 막기 위해 
	scanf("%[^\n]", newstd.data.name); while (getchar() != '\n');	// %[^\n]은 공백을 받기 위함. \n이 나오기 전까지 입력을 받음
	printf("Gender : ");
	scanf("%c", &(newstd.data.gender)); while (getchar() != '\n');	
	printf("Dept : ");
	scanf("%s", newstd.data.dept); while (getchar() != '\n');
	printf("Group : ");
	scanf("%d", &(newstd.data.group)); while (getchar() != '\n');
	
	G_AddNode(slist, newstd, newstd.data.group - 1);	// newstd에 받은 정보를 해당 그룹의 끝에 저장 
	
	printf("Player successfully added to group %d.\n\n", newstd.data.group);
}

void P_Remove(LIST *slist) {
	NODE* temp;
	int i, j, ID, count = 0;
	
	printf("StudentID : ");
	scanf("%d", &ID);
	
	for(i = 0; i < 5; i++) {	// 해당 학번을 가진 학생이 있는지 검사 
		temp = slist->tail[i]->next;	// 각 group의 head 부터 검사
		for(j = 0; j < (slist->count[i]); j++) {	// 해당 group의 학생 수 만큼 검사 
			if((temp->data.stdID) == ID) {	// 학생이 존재할 때
				break;
			}
			else
			count++; 
			temp = temp->next;
		}
		if((temp->data.stdID) == ID) {	// 학생이 존재할 때 (for문을 한번 더 탈출) 
			break;
		}
	}
	
	if(count == (slist->count[0] + slist->count[1] + slist->count[2] + slist->count[3] + slist->count[4])) {	// count가 전체 학생 수와 같다면 해당 학생은 존재x 
		printf("Player does not exist!\n");
		return;
	}
	
	G_DelNode(slist, ID, temp->data.group - 1);	// 존재하면 목록에서 제거 
	
	printf("Player successfully removed from group %d.\n", temp->data.group);
}

void P_Play(LIST *slist) {
	FILE* fp;
	char a, fname[31];	// 파일 명 30자 제한 + \0 고려해 31칸 배열을 선언 
	int i, nextgroup, nextplayer;
	NODE *temp, *del;
	
	scanf("%c", &a); // 파일명을 입력했는지 검사. 만약 a가 '\n'이면 파일명을 입력하지 않았고, ' '(공백)이면 파일명을 입력한 것 
	if(a == '\n') {
				if((fp = fopen("game.txt", "r")) == NULL) {	// game.txt가 없을 때 if 내부 실행. 있다면 if 내부 실행 없이 fp에 정보가 저장됨 
			printf("Cannot open file!\n");
			return;
			}
	}
	
	else {
			scanf("%s", fname);
	if((fp = fopen(fname, "r")) == NULL) {	// 파일이 있는지 검사. 있다면 if 내부 실행 없이 fp에 정보가 저장됨 
		if((fp = fopen("game.txt", "r")) == NULL) {	// game.txt가 없을 때 if 내부 실행. 있다면 if 내부 실행 없이 fp에 정보가 저장됨 
			printf("Cannot open file!\n");
			return;
			}
	}
	}

	
	for(i = 0; i < 5; i++) {	// 전체 학생 수가 0인지 검사 
		if(slist->count[i] != 0) break;	// 0이 아니면 학생이 있으므로 통과 
	}
	if(i == 5) {	// i가 5면 모든 group의 count가 0이라는 뜻이므로 학생이 0명 있음(i가 5면 위의 break를 한번도 실행하지 않았다는 것.) 
		printf("And then there were none...\n");
		return;
	}
	
	for(i = 0; i < 5; i++) {	// 시작 술래 지정하기 
		if(slist->count[i] == 0) continue;	// k번째 group이 비었으면 k + 1번째 그룹의 head가 술래가 되도록 하기 위함 
		else {
			temp = slist->tail[i]->next;	// 처음으로 학생이 존재하는 group의 head 지정 
			slist->tail[i]->next = temp->next;	// 그 head를 group에서 밖으로 빼냄 
			
			(slist->count[i])--;	// 수건을 돌리다가 어떤 group에 있는 학생이 out되어도 학생 수가 줄어드는 group은 첫 술래가 있는 group 
			break;
		}
	}
	
	printf("From\t\t\t\t\tTo\n");	// 간격 조절용 \t 
	while(fscanf(fp, "%d%d", &nextgroup, &nextplayer) != EOF) {	// 파일이 끝날 때 까지 반복 
		if(slist->count[nextgroup - 1] == 0) {	// 넘길 group의 학생 수가 0명이라면 넘기지 않고 다음 파일 정보 읽기 
			continue;
		}
		
		if(nextplayer != slist->count[nextgroup - 1]) {	// nextplayer 값이 그 그룹의 최대 학생 수를 넘으면 (nextplayer / nextgroup의 학생 수)의 나머지가 nextplayer가 되어야 하므로 
			if(slist->count[nextgroup - 1] == 1) {	// 만약 nextgroup에 있는 학생 수가 1명이라면 nextplayer의 값은 1. 만약 이 조건이 없다면 학생수가 1일 때 nextplayer의 값이 0이 될 수 있다 
				nextplayer = 1;
			}
			else if(nextplayer % (slist->count[nextgroup - 1]) == 0) {
				nextplayer = slist->count[nextgroup - 1];
			}	// 만약 nextplayer가 group에 있는 학생 수의 배수면  nextplayer의 값을 group에 있는 학생 수로 사용. 아니면 nextplayer가 0이 되어 프로그램 오류 발생 
			else {
				nextplayer = nextplayer % (slist->count[nextgroup - 1]);	// 두 값이 다르면 위의 알고리즘대로 작성해 사용 
			}
		}
		del = slist->tail[nextgroup - 1];	// 술래로 지정할 학생이 있는 group의 tail로 이동 
		
		for(i = 0; i < nextplayer; i++) {	//  술래로 지정할 학생이 있는 node 바로 앞까지 del을 옮김 
			del = del->next;
		}
		printf("\n%-21s (Group %d)\t  ->\t%-21s (Group %d)\n", temp->data.name, temp->data.group, del->data.name, del->data.group);	// temp는 현 술래의 정보를, del은 술래가 될 학생의 정보를 들고 있음 
		
		temp = G_Out(slist, temp, nextgroup - 1, nextplayer);	// 술래가 될 사람을 술래로 만들고 현 술래를 빠진 자리에 넣음 
	}
	
	printf("\n\n");
	printf("Game over!\n\n");
	printf("%s (Group %d) is out.\n\n", temp->data.name, temp->data.group);	// 술래 탈락
	free(temp);	// 술래 메모리 할당 해제 
}

void P_Quit(LIST *slist) {
	FILE *fp;
	int i, j;
	NODE *temp, *pre;
	
	if((fp = fopen("result_game.txt", "w")) == NULL) {	// 파일 쓰기모드로 열기 실패 시 if 내부 실행 
		printf("Cannot save the result!\n");
		return;
	}
	
	for(i = 0; i < 5; i++) {	// 결과를 파일로 저장 
		temp = slist->tail[i]->next;	// 각 조 head부터 시작 
		for(j = 0; j < (slist->count[i]); j++) {	// 각 조 학생 수 만큼 반복 
			fprintf(fp, "%s\t%c\t%s\t%d\t%d\n", temp->data.name, temp->data.gender, temp->data.dept, temp->data.stdID, temp->data.group);
			temp = temp->next;
		}
	}
	
	for(i = 0; i < 5; i++) {	// 메모리 할당 해제
		temp = slist->tail[i]->next;	// 각 조 head부터 free 
		for(j = 0; j < (slist->count[i]); j++) {	// 각 조 학생 수 만큼 free 
			pre = temp;
			pre = pre->next;
			free(temp);	// 할당 해제 
			temp = pre;	// 다음 칸으로 이동 
		}
	}
	
	free(slist->tail);	// main에서 받은 메모리 해제 
	free(slist->count);	// main에서 받은 메모리 해제 
	free(slist);	// main에서 받은 메모리 해제 
	printf("File saved. Bye!\n");
	
	fclose(fp);
	
	exit(0);	// 종료 
}

void line(int a) {
	int i;
	for (i = 0; i < a; i++) {
		printf("="); //원하는 개수(a)만큼 '=' 표시 
	}
}
