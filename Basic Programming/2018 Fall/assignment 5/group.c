#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

void G_LoadData(LIST *slist, char *argv[]) {
	FILE* fp;
	int i, fsize;
	NODE std;
	
	if((fp = fopen(argv[1], "r")) == NULL) {	// argv 사용 
		if((fp = fopen("players.txt", "r")) == NULL) {	// argv에 저장된 문자열을 이름으로 하는 파일이 없으면 players.txt 열기 
			printf("Cannot open file!\n");	// 없으면 오류 
			exit(0);
		}
	}
	
	fseek(fp, 0, SEEK_END);	// 파일 포인터를 맨 끝으로 옮김 
	if((fsize = ftell(fp)) == 0) {	// 파일 크기가 0일 경우 
		fclose(fp);
		return;	// 아무 정보가 없으므로 정보를 저장하지 않고 return 
	}
	fseek(fp, 0, SEEK_SET);	// 다시 파일 포인터를 맨 앞으로 옮김 
	
	while(1) {
		for(i = 0; (std.data.name[i - 1]) != '\t'; i++) {	// \t를 만날 때 까지 반복. 만약 \t를 읽어 문자열에 저장했다면 i가 1 증가하므로 (i - 1)을 해야 올바른 for문 탈출 조건이 됨 
			fscanf(fp, "%c", &(std.data.name[i]));	// 문자열에 저장 
		}
		std.data.name[i - 1] = '\0';	// (i - 2)번째 까지 문자가 잘 들어갔고, (i - 1)번째는 \t이 있다. 그런데 (i - 1)번째는 \0을 저장해야 하므로 값을 바꿔 준다 
		fscanf(fp, "%c", &(std.data.gender));	// 성별 저장 
		fscanf(fp, "%*c");	// \t 받아서 무시 
		for(i = 0; (std.data.dept[i - 1]) != '\t'; i++) {	// 이름을 저장하는 방법과 같은 알고리즘 
			fscanf(fp, "%c", &(std.data.dept[i]));
		}
		std.data.dept[i - 1] = '\0';
		fscanf(fp, "%d %d", &(std.data.stdID), &(std.data.group));	// 학번, group 정보 저장 
		fscanf(fp, "%*c");	// \n 받아서 무시(1)
		
		G_AddNode(slist, std, std.data.group - 1);	// 학생 정보 저장용 node를 맞는 group에 추가 
		
		if(feof(fp)) break;	// feof : 파일 포인터의 위치가 값을 읽을 수 없는 위치에 있다면 1을 반환
		// 파일 끝에서는 0을 반환하나, 마지막 줄에는 \n이 없어 (1)에서 파일포인터가 값을 읽을 수 없는 상태가 되므로 정상 작동 
		// 만약 파일의 마지막에 \n이 있다면 프로그램 오류 발생
	}
	
	fclose(fp);
}

void G_AddNode(LIST *slist, NODE node, int n) {
	NODE *pre = slist->tail[n];
	NODE *newnode = (NODE*)malloc(sizeof(NODE));
	int i;
	
	newnode->data = node.data;
	if((slist->count[n]) == 0) {	// group에 학생이 없다면 추가되는 node는 자기 자신을 가리켜야 함 
		newnode->next = newnode;	// 자기 자신 가리키기 
		slist->tail[n] = newnode;	// node가 1개이므로 자기 자신이 tail이 됨 
	}
	else {
		
		newnode->next = pre->next;	// newnode를 pre(tail과 같다)의 다음에 추가
		pre->next = newnode;
		slist->tail[n] = newnode;	//  newnode가 새로운 tail이 되어야 함 
	}
	
	(slist->count[n])++;	// 목록 학생 수 1 증가 
}

NODE* G_Out(LIST *slist, NODE* temp, int nextgroup, int nextplayer) {
	NODE *pre = slist->tail[nextgroup];
	NODE *del = slist->tail[nextgroup];
	int i;
	
	for(i = 0; i < (nextplayer - 1); i++) {	// 나갈 사람 앞 node까지 pre 이동 
		pre = pre->next;
	}
	del = pre->next;	// pre의 다음 node를 제거(내쫒기)할 대상으로 지정 
	pre->next = temp;	// pre의 다음 node가 temp(술래) 
	temp->next = del->next;	// 술래가 될 사람(del)이 가진 다음 node 정보를 temp에 줌 
	temp->data.group = del->data.group;	// group이 변할 수도 있으므로 del의 group 정보를 temp에 줌 
	// 현재 del이 술래가 되었고(group에서 쫒겨남) temp가 자리를 차지 
	if(nextplayer == slist->count[nextgroup]) {	// 만약 끝에 temp가 들어가면 temp가 tail이 되어야 함 
		slist->tail[nextgroup] = temp;
	}
	
	return del;	// 술래 return 
}


void G_DelNode(LIST *slist, int ID, int n) {
	NODE *pre = slist->tail[n]->next;
	NODE *del = slist->tail[n];
	int count = 0, i;
	
	while(pre->data.stdID != ID) {	// 같은 학번이 있을 때 까지 pre 이동 
		pre = pre->next;
		count++;
	}
	
	pre = slist->tail[n];
	for(i = 0; i < count; i++) {	// 위에서 센 값 만큼 pre 이동 
		pre = pre->next;
	}
	// pre가 지울 node 뒤에 위치 
	del = pre->next;
	pre->next = del->next;	// del을 목록에서 빼냄 
	free(del);	// 할당 해제 
	
	if(count == slist->count[n] - 1) {	// 마지막(tail)이 지워졌을 때 
		slist->tail[n] = pre;	// pre가 tail이 됨 
	}
	
	(slist->count[n])--;	// 학생 1 감소 
}
