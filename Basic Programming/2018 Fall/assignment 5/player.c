#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"

void P_Show(LIST *slist, int n) { 
	int i;
	NODE *temp;
	
	line(28);
	printf(" GROUP %d ", n + 1);	// n�� �迭�� �� ����(0 ~ 4)�� �ޱ� ������ ���ϴ� �׷� ���ڸ� ����ϰ� ������ n + 1�� ����� �Ѵ� 
	line(28); printf("\n");
	
	temp = slist->tail[n];	// temp�� ������ group�� tail�� �̵� 
	for(i = 0; i < (slist->count[n]); i++) {
		temp = temp->next;	// temp�� ������ group�� head���� ������ tail���� ��
		printf("\n%-21s\t%c\t%-20s\t%d\n",temp->data.name, temp->data.gender, temp->data.dept, temp->data.stdID);
		
	}
	printf("\n");
}

void P_Add(LIST *slist) {
	NODE newstd, *temp;	// newstd�� �ܼ� ���� ����, *temp�� linked list ����� 
	int i, j;
	
	printf("StudentID : ");
	scanf("%d", &(newstd.data.stdID));
	
	
	for(i = 0; i < 5; i++) {	// �ش� �й��� ���� �л��� �ִ��� �˻� 
		temp = slist->tail[i]->next;	// �� group�� head ���� �˻� 
		for(j = 0; j < (slist->count[i]); j++) {	// �ش� group�� �л� �� ��ŭ �˻� 
			if((temp->data.stdID) == (newstd.data.stdID)) {	// �л��� ������ �� 
				printf("Player already exists!\n");
				printf("name\t\t\tGender\tdept\t\t\tStudentID\tGroup\n");
				printf("%-21s\t%c\t%-20s\t%d\t%d\n",temp->data.name, temp->data.gender, temp->data.dept, temp->data.stdID, temp->data.group);	// ���� ����, �ڸ��� ���� 
				return;
			}
			else
			temp = temp->next;
		}
	}
	
	printf("Name : ");
	while (getchar() != '\n');	// ���� �ʱ�ȭ. ���� ������ ������ �߻��ϴ� ���� ���� ���� 
	scanf("%[^\n]", newstd.data.name); while (getchar() != '\n');	// %[^\n]�� ������ �ޱ� ����. \n�� ������ ������ �Է��� ����
	printf("Gender : ");
	scanf("%c", &(newstd.data.gender)); while (getchar() != '\n');	
	printf("Dept : ");
	scanf("%s", newstd.data.dept); while (getchar() != '\n');
	printf("Group : ");
	scanf("%d", &(newstd.data.group)); while (getchar() != '\n');
	
	G_AddNode(slist, newstd, newstd.data.group - 1);	// newstd�� ���� ������ �ش� �׷��� ���� ���� 
	
	printf("Player successfully added to group %d.\n\n", newstd.data.group);
}

void P_Remove(LIST *slist) {
	NODE* temp;
	int i, j, ID, count = 0;
	
	printf("StudentID : ");
	scanf("%d", &ID);
	
	for(i = 0; i < 5; i++) {	// �ش� �й��� ���� �л��� �ִ��� �˻� 
		temp = slist->tail[i]->next;	// �� group�� head ���� �˻�
		for(j = 0; j < (slist->count[i]); j++) {	// �ش� group�� �л� �� ��ŭ �˻� 
			if((temp->data.stdID) == ID) {	// �л��� ������ ��
				break;
			}
			else
			count++; 
			temp = temp->next;
		}
		if((temp->data.stdID) == ID) {	// �л��� ������ �� (for���� �ѹ� �� Ż��) 
			break;
		}
	}
	
	if(count == (slist->count[0] + slist->count[1] + slist->count[2] + slist->count[3] + slist->count[4])) {	// count�� ��ü �л� ���� ���ٸ� �ش� �л��� ����x 
		printf("Player does not exist!\n");
		return;
	}
	
	G_DelNode(slist, ID, temp->data.group - 1);	// �����ϸ� ��Ͽ��� ���� 
	
	printf("Player successfully removed from group %d.\n", temp->data.group);
}

void P_Play(LIST *slist) {
	FILE* fp;
	char a, fname[31];	// ���� �� 30�� ���� + \0 ����� 31ĭ �迭�� ���� 
	int i, nextgroup, nextplayer;
	NODE *temp, *del;
	
	scanf("%c", &a); // ���ϸ��� �Է��ߴ��� �˻�. ���� a�� '\n'�̸� ���ϸ��� �Է����� �ʾҰ�, ' '(����)�̸� ���ϸ��� �Է��� �� 
	if(a == '\n') {
				if((fp = fopen("game.txt", "r")) == NULL) {	// game.txt�� ���� �� if ���� ����. �ִٸ� if ���� ���� ���� fp�� ������ ����� 
			printf("Cannot open file!\n");
			return;
			}
	}
	
	else {
			scanf("%s", fname);
	if((fp = fopen(fname, "r")) == NULL) {	// ������ �ִ��� �˻�. �ִٸ� if ���� ���� ���� fp�� ������ ����� 
		if((fp = fopen("game.txt", "r")) == NULL) {	// game.txt�� ���� �� if ���� ����. �ִٸ� if ���� ���� ���� fp�� ������ ����� 
			printf("Cannot open file!\n");
			return;
			}
	}
	}

	
	for(i = 0; i < 5; i++) {	// ��ü �л� ���� 0���� �˻� 
		if(slist->count[i] != 0) break;	// 0�� �ƴϸ� �л��� �����Ƿ� ��� 
	}
	if(i == 5) {	// i�� 5�� ��� group�� count�� 0�̶�� ���̹Ƿ� �л��� 0�� ����(i�� 5�� ���� break�� �ѹ��� �������� �ʾҴٴ� ��.) 
		printf("And then there were none...\n");
		return;
	}
	
	for(i = 0; i < 5; i++) {	// ���� ���� �����ϱ� 
		if(slist->count[i] == 0) continue;	// k��° group�� ������� k + 1��° �׷��� head�� ������ �ǵ��� �ϱ� ���� 
		else {
			temp = slist->tail[i]->next;	// ó������ �л��� �����ϴ� group�� head ���� 
			slist->tail[i]->next = temp->next;	// �� head�� group���� ������ ���� 
			
			(slist->count[i])--;	// ������ �����ٰ� � group�� �ִ� �л��� out�Ǿ �л� ���� �پ��� group�� ù ������ �ִ� group 
			break;
		}
	}
	
	printf("From\t\t\t\t\tTo\n");	// ���� ������ \t 
	while(fscanf(fp, "%d%d", &nextgroup, &nextplayer) != EOF) {	// ������ ���� �� ���� �ݺ� 
		if(slist->count[nextgroup - 1] == 0) {	// �ѱ� group�� �л� ���� 0���̶�� �ѱ��� �ʰ� ���� ���� ���� �б� 
			continue;
		}
		
		if(nextplayer != slist->count[nextgroup - 1]) {	// nextplayer ���� �� �׷��� �ִ� �л� ���� ������ (nextplayer / nextgroup�� �л� ��)�� �������� nextplayer�� �Ǿ�� �ϹǷ� 
			if(slist->count[nextgroup - 1] == 1) {	// ���� nextgroup�� �ִ� �л� ���� 1���̶�� nextplayer�� ���� 1. ���� �� ������ ���ٸ� �л����� 1�� �� nextplayer�� ���� 0�� �� �� �ִ� 
				nextplayer = 1;
			}
			else if(nextplayer % (slist->count[nextgroup - 1]) == 0) {
				nextplayer = slist->count[nextgroup - 1];
			}	// ���� nextplayer�� group�� �ִ� �л� ���� �����  nextplayer�� ���� group�� �ִ� �л� ���� ���. �ƴϸ� nextplayer�� 0�� �Ǿ� ���α׷� ���� �߻� 
			else {
				nextplayer = nextplayer % (slist->count[nextgroup - 1]);	// �� ���� �ٸ��� ���� �˰����� �ۼ��� ��� 
			}
		}
		del = slist->tail[nextgroup - 1];	// ������ ������ �л��� �ִ� group�� tail�� �̵� 
		
		for(i = 0; i < nextplayer; i++) {	//  ������ ������ �л��� �ִ� node �ٷ� �ձ��� del�� �ű� 
			del = del->next;
		}
		printf("\n%-21s (Group %d)\t  ->\t%-21s (Group %d)\n", temp->data.name, temp->data.group, del->data.name, del->data.group);	// temp�� �� ������ ������, del�� ������ �� �л��� ������ ��� ���� 
		
		temp = G_Out(slist, temp, nextgroup - 1, nextplayer);	// ������ �� ����� ������ ����� �� ������ ���� �ڸ��� ���� 
	}
	
	printf("\n\n");
	printf("Game over!\n\n");
	printf("%s (Group %d) is out.\n\n", temp->data.name, temp->data.group);	// ���� Ż��
	free(temp);	// ���� �޸� �Ҵ� ���� 
}

void P_Quit(LIST *slist) {
	FILE *fp;
	int i, j;
	NODE *temp, *pre;
	
	if((fp = fopen("result_game.txt", "w")) == NULL) {	// ���� ������� ���� ���� �� if ���� ���� 
		printf("Cannot save the result!\n");
		return;
	}
	
	for(i = 0; i < 5; i++) {	// ����� ���Ϸ� ���� 
		temp = slist->tail[i]->next;	// �� �� head���� ���� 
		for(j = 0; j < (slist->count[i]); j++) {	// �� �� �л� �� ��ŭ �ݺ� 
			fprintf(fp, "%s\t%c\t%s\t%d\t%d\n", temp->data.name, temp->data.gender, temp->data.dept, temp->data.stdID, temp->data.group);
			temp = temp->next;
		}
	}
	
	for(i = 0; i < 5; i++) {	// �޸� �Ҵ� ����
		temp = slist->tail[i]->next;	// �� �� head���� free 
		for(j = 0; j < (slist->count[i]); j++) {	// �� �� �л� �� ��ŭ free 
			pre = temp;
			pre = pre->next;
			free(temp);	// �Ҵ� ���� 
			temp = pre;	// ���� ĭ���� �̵� 
		}
	}
	
	free(slist->tail);	// main���� ���� �޸� ���� 
	free(slist->count);	// main���� ���� �޸� ���� 
	free(slist);	// main���� ���� �޸� ���� 
	printf("File saved. Bye!\n");
	
	fclose(fp);
	
	exit(0);	// ���� 
}

void line(int a) {
	int i;
	for (i = 0; i < a; i++) {
		printf("="); //���ϴ� ����(a)��ŭ '=' ǥ�� 
	}
}
