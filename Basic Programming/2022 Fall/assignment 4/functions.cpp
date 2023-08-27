#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"


void show_fn(NODE* head) {
	NODE* h_node = head->next;
	int pos = 1;

	printf("                                       PLAYLIST                                     \n");
	printf("====================================================================================\n");
	printf(" No. %14s    %14s    %14s    %14s\n", "Title", "Artist", "Volume", "Preference");
	printf("====================================================================================\n");
	printf("------------------------------------------------------------------------------------\n");
	if (h_node == NULL)
		printf("Empty Playlist!\n");
	else {
		for (h_node = head->next; h_node != NULL; h_node = h_node->next) {
			printf("#%-3d|%14s   |%14s   |%11.2f MB   |%8.2f\n", pos, h_node->data.title, h_node->data.artist, h_node->data.size, h_node->data.prefer);
			pos++;
		}
	}
	printf("------------------------------------------------------------------------------------\n");
	printf("Total number of songs: %d\n", head->count);
	printf("Storage: %.2f MB\n", head->storage);
	printf("====================================================================================\n\n");
	return;
}

void show_favorites_fn(NODE* head) {
	NODE* h_node = head->next;
	NODE* favors = NULL, * f_node = NULL;
	MUSIC null_data = { "", "", 0, -1 };
	int mus_num = 0, i = 0, pos = 1;

	if (head->count == 0) {
		printf("ERROR: # of music = 0\n");
		return;
	}

	printf("상위 몇 개의 음악을 추출할까요? >> ");
	scanf("%d", &mus_num);
	while (getchar() != '\n');
	if (mus_num > head->count) {
		printf("추출하고자 하는 음악의 수가 전체 음악의 수보다 많습니다.\n\n");
		return;
	}
	else if (mus_num < 1) {
		printf("추출하고자 하는 음악의 수는 1 이상이어야 합니다.\n\n");
		return;
	}

	favors = (NODE*)malloc(sizeof(NODE));
	if (favors == NULL) {
		printf("Allocation failed.\n");
		exit(0);
	}
	favors->next = NULL;
	favors->count = 0;
	favors->storage = 0;

	for (i = 0; i < mus_num; i++)
		add_node(favors, null_data, pos);

	for (h_node = head->next; h_node != NULL; h_node = h_node->next) {
		pos = 1;
		for (f_node = favors; f_node->next != NULL; f_node = f_node->next) {
			if (h_node->data.prefer > f_node->next->data.prefer) {
				add_node(favors, h_node->data, pos);
				remove_node(favors, mus_num + 1);
				break;
			}
			pos++;
		}
	}

	show_fn(favors);
	for (i = 0; i < mus_num; i++)
		remove_node(favors, 1);
	free(favors);
	return;
}

void add_fn(NODE* head) {
	NODE* h_node = head->next;
	MUSIC info = { "", "", 0, -1 };
	int pos = 1;

	printf("추가할 음악의 타이틀을 입력해주세요. >> ");
	scanf("%s", info.title);
	while (getchar() != '\n');
	printf("추가할 음악의 아티스트를 입력해주세요. >> ");
	scanf("%s", info.artist);
	while (getchar() != '\n');
	printf("추가할 음악의 용량을 입력해주세요. >> ");
	scanf("%f", &info.size);
	while (getchar() != '\n');
	printf("추가할 음악의 선호도를 입력해주세요. >> ");
	scanf("%f", &info.prefer);
	while (getchar() != '\n');

	add_alphabetical(head, info);
	printf("\n");
	return;
}

void delete_fn(NODE* head) {
	char del_title[MLEN] = "";
	NODE* h_node = head->next;
	int pos = 1;

	printf("삭제할 음악의 타이틀을 입력해주세요. >> ");
	scanf("%s", del_title);
	while (getchar() != '\n');

	while (h_node != NULL) {
		if (strcmp(h_node->data.title, del_title) != 0) {
			h_node = h_node->next;
			pos++;
		}
		else
			break;
	}

	if (h_node == NULL) {
		printf("플레이리스트에 해당 음악(%s)이 없습니다.\n\n", del_title);
		return;
	}

	printf("\n");
	printf("====================================================================================\n");
	printf(" No. %14s    %14s    %14s    %14s\n", "Title", "Artist", "Volume", "Preference");
	printf("------------------------------------------------------------------------------------\n");
	printf("#%-3d|%14s   |%14s   |%11.2f MB   |%8.2f\n", pos, h_node->data.title, h_node->data.artist, h_node->data.size, h_node->data.prefer);
	printf("------------------------------------------------------------------------------------\n");
	printf("위 음악이 삭제되었습니다.\n\n");
	remove_node(head, pos);
	return;
}

void exit_fn(NODE* head) {
	FILE* fp = NULL;
	NODE* h_node = head->next;
	int pos = 1;
	char fname[FNAME] = "";

	printf("저장할 파일명을 입력해주세요. >> ");
	scanf("%s", fname);
	while (getchar() != '\n');
	fp = fopen(fname, "w");
	for (h_node = head->next; h_node != NULL; h_node = head->next) {
		fprintf(fp, "%s\t%s\t%.2f\t%.2f\n", h_node->data.title, h_node->data.artist, h_node->data.size, h_node->data.prefer);
		remove_node(head, pos);
	}
	free(head);
	printf("프로그램을 종료합니다.\n\n");
	return;
}

void read_list(NODE* head) {
	FILE* fp = NULL;
	MUSIC info = { "", "", 0, -1 };
	char fname[FNAME] = "";

	printf("음악 리스트 파일 이름을 입력해주세요. >> ");
	while (1) {
		scanf("%s", fname);
		while (getchar() != '\n');
		fp = fopen(fname, "r");
		if (fp == NULL)
			printf("유효하지 않은 파일입니다. 음악 리스트 파일을 다시 입력해주세요. >> ");
		else
			break;
	}

	while (fscanf(fp, "%s\t%s\t%f\t%f", info.title, info.artist, &info.size, &info.prefer) == 4)
		add_alphabetical(head, info);
	printf("\n");
	return;
}

void copy_data(NODE* to, MUSIC from) {
	strcpy(to->data.title, from.title);
	strcpy(to->data.artist, from.artist);
	to->data.size = from.size;
	to->data.prefer = from.prefer;
	return;
}

void add_node(NODE* head, MUSIC info, int pos) {
	NODE* prev_node = head, * now_node = prev_node->next;
	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	int i;

	if (new_node == NULL) {
		printf("Allocation failed.\n");
		exit(0);
	}

	new_node->next = NULL;
	for (i = 1; i < pos; i++)
		prev_node = prev_node->next;

	now_node = prev_node->next;
	prev_node->next = new_node;
	new_node->next = now_node;

	copy_data(new_node, info);
	head->count++;
	head->storage += info.size;
	return;
}

void remove_node(NODE* head, int pos) {
	NODE* prev_node = head, * now_node = prev_node->next;
	int i;

	for (i = 1; i < pos; i++)
		prev_node = prev_node->next;

	now_node = prev_node->next;
	prev_node->next = now_node->next;

	head->count--;
	head->storage -= now_node->data.size;
	free(now_node);
	return;
}

void add_alphabetical(NODE* head, MUSIC info) {
	NODE* h_node = head->next;
	int pos = 1;

	if (head->storage + info.size > CAPACITY) {
		printf("용량 초과! 음악(%s)은 추가되지 않았습니다.\n", info.title);
		return;
	}

	for (h_node = head->next; h_node != NULL; h_node = h_node->next) {
		if (strcmp(h_node->data.title, info.title) < 0)
			pos++;
		else if (strcmp(h_node->data.title, info.title) == 0) {
			printf("해당 음악이 이미 플레이리스트 내에 존재합니다.\n");
			return;
		}
		else
			break;
	}
	add_node(head, info, pos);
	return;
}