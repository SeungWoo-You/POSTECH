#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

void G_LoadData(LIST *slist, char *argv[]) {
	FILE* fp;
	int i, fsize;
	NODE std;
	
	if((fp = fopen(argv[1], "r")) == NULL) {	// argv ��� 
		if((fp = fopen("players.txt", "r")) == NULL) {	// argv�� ����� ���ڿ��� �̸����� �ϴ� ������ ������ players.txt ���� 
			printf("Cannot open file!\n");	// ������ ���� 
			exit(0);
		}
	}
	
	fseek(fp, 0, SEEK_END);	// ���� �����͸� �� ������ �ű� 
	if((fsize = ftell(fp)) == 0) {	// ���� ũ�Ⱑ 0�� ��� 
		fclose(fp);
		return;	// �ƹ� ������ �����Ƿ� ������ �������� �ʰ� return 
	}
	fseek(fp, 0, SEEK_SET);	// �ٽ� ���� �����͸� �� ������ �ű� 
	
	while(1) {
		for(i = 0; (std.data.name[i - 1]) != '\t'; i++) {	// \t�� ���� �� ���� �ݺ�. ���� \t�� �о� ���ڿ��� �����ߴٸ� i�� 1 �����ϹǷ� (i - 1)�� �ؾ� �ùٸ� for�� Ż�� ������ �� 
			fscanf(fp, "%c", &(std.data.name[i]));	// ���ڿ��� ���� 
		}
		std.data.name[i - 1] = '\0';	// (i - 2)��° ���� ���ڰ� �� ����, (i - 1)��°�� \t�� �ִ�. �׷��� (i - 1)��°�� \0�� �����ؾ� �ϹǷ� ���� �ٲ� �ش� 
		fscanf(fp, "%c", &(std.data.gender));	// ���� ���� 
		fscanf(fp, "%*c");	// \t �޾Ƽ� ���� 
		for(i = 0; (std.data.dept[i - 1]) != '\t'; i++) {	// �̸��� �����ϴ� ����� ���� �˰��� 
			fscanf(fp, "%c", &(std.data.dept[i]));
		}
		std.data.dept[i - 1] = '\0';
		fscanf(fp, "%d %d", &(std.data.stdID), &(std.data.group));	// �й�, group ���� ���� 
		fscanf(fp, "%*c");	// \n �޾Ƽ� ����(1)
		
		G_AddNode(slist, std, std.data.group - 1);	// �л� ���� ����� node�� �´� group�� �߰� 
		
		if(feof(fp)) break;	// feof : ���� �������� ��ġ�� ���� ���� �� ���� ��ġ�� �ִٸ� 1�� ��ȯ
		// ���� �������� 0�� ��ȯ�ϳ�, ������ �ٿ��� \n�� ���� (1)���� ���������Ͱ� ���� ���� �� ���� ���°� �ǹǷ� ���� �۵� 
		// ���� ������ �������� \n�� �ִٸ� ���α׷� ���� �߻�
	}
	
	fclose(fp);
}

void G_AddNode(LIST *slist, NODE node, int n) {
	NODE *pre = slist->tail[n];
	NODE *newnode = (NODE*)malloc(sizeof(NODE));
	int i;
	
	newnode->data = node.data;
	if((slist->count[n]) == 0) {	// group�� �л��� ���ٸ� �߰��Ǵ� node�� �ڱ� �ڽ��� �����Ѿ� �� 
		newnode->next = newnode;	// �ڱ� �ڽ� ����Ű�� 
		slist->tail[n] = newnode;	// node�� 1���̹Ƿ� �ڱ� �ڽ��� tail�� �� 
	}
	else {
		
		newnode->next = pre->next;	// newnode�� pre(tail�� ����)�� ������ �߰�
		pre->next = newnode;
		slist->tail[n] = newnode;	//  newnode�� ���ο� tail�� �Ǿ�� �� 
	}
	
	(slist->count[n])++;	// ��� �л� �� 1 ���� 
}

NODE* G_Out(LIST *slist, NODE* temp, int nextgroup, int nextplayer) {
	NODE *pre = slist->tail[nextgroup];
	NODE *del = slist->tail[nextgroup];
	int i;
	
	for(i = 0; i < (nextplayer - 1); i++) {	// ���� ��� �� node���� pre �̵� 
		pre = pre->next;
	}
	del = pre->next;	// pre�� ���� node�� ����(���i��)�� ������� ���� 
	pre->next = temp;	// pre�� ���� node�� temp(����) 
	temp->next = del->next;	// ������ �� ���(del)�� ���� ���� node ������ temp�� �� 
	temp->data.group = del->data.group;	// group�� ���� ���� �����Ƿ� del�� group ������ temp�� �� 
	// ���� del�� ������ �Ǿ���(group���� �i�ܳ�) temp�� �ڸ��� ���� 
	if(nextplayer == slist->count[nextgroup]) {	// ���� ���� temp�� ���� temp�� tail�� �Ǿ�� �� 
		slist->tail[nextgroup] = temp;
	}
	
	return del;	// ���� return 
}


void G_DelNode(LIST *slist, int ID, int n) {
	NODE *pre = slist->tail[n]->next;
	NODE *del = slist->tail[n];
	int count = 0, i;
	
	while(pre->data.stdID != ID) {	// ���� �й��� ���� �� ���� pre �̵� 
		pre = pre->next;
		count++;
	}
	
	pre = slist->tail[n];
	for(i = 0; i < count; i++) {	// ������ �� �� ��ŭ pre �̵� 
		pre = pre->next;
	}
	// pre�� ���� node �ڿ� ��ġ 
	del = pre->next;
	pre->next = del->next;	// del�� ��Ͽ��� ���� 
	free(del);	// �Ҵ� ���� 
	
	if(count == slist->count[n] - 1) {	// ������(tail)�� �������� �� 
		slist->tail[n] = pre;	// pre�� tail�� �� 
	}
	
	(slist->count[n])--;	// �л� 1 ���� 
}
