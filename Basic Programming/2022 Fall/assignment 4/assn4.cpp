#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

#define BUFFER 100

int get_cmd();


int main() {
	NODE* head = (NODE*)malloc(sizeof(NODE));
	int choice = -1;
	
	if (head == NULL) {
		printf("Allocation failed.\n");
		exit(0);
	}
	head->next = NULL;
	head->count = 0;
	head->storage = 0;

	read_list(head);
	while (choice != 0) {
		choice = get_cmd();
		switch (choice) {
		case 1:
			show_fn(head);
			break;
		case 2:
			show_favorites_fn(head);
			break;
		case 3:
			add_fn(head);
			break;
		case 4:
			delete_fn(head);
			break;
		case 0:
			exit_fn(head);
			break;
		default:
			break;
		}
	}

	return 0;
}

int get_cmd() {
	char cmd[BUFFER] = "";

	while (1) {
		printf("명령어를 입력해주세요. >> ");
		scanf("%s", cmd);
		while (getchar() != '\n');
		if (strcmp(cmd, "show") == 0)
			return 1;
		else if (strcmp(cmd, "show_favorites") == 0)
			return 2;
		else if (strcmp(cmd, "add") == 0)
			return 3;
		else if (strcmp(cmd, "delete") == 0)
			return 4;
		else if (strcmp(cmd, "exit") == 0)
			return 0;
		else
			printf("유효하지 않은 명령어입니다.\n\n");
	}
	return -1;
}