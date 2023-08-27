#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

void U_user_add(UNODE* ulist, TNODE* tlist) {
	UNODE newuser, * temp;
	TNODE trader;

	printf("Enter user name >> ");
	if (scanf_s("%s", newuser.data.name, sizeof(newuser.data.name)) == 0) {
		printf("User add failed\n");
		while (getchar() != '\n');
		return;
	}

	temp = ulist->next;
	temp = find_user(temp, newuser.data.name);
	if (temp != NULL) {
		printf("Alread exist\n");
		return;
	}
	
	newuser.data.coin = 0;

	U_add_node(ulist, newuser);
	
	if (ulist->next == NULL) {
		printf("error\n");
		return;
	}

	if (tlist->next == NULL) {
		strcpy_s(trader.data.sender, sizeof(trader.data.sender), ulist->next->data.name);
		trader.data.coin = ulist->next->data.coin;
		trader.data.check = 0;
		T_add_node(tlist, trader);
	}


	if (tlist->next == NULL) {
		printf("error\n");
		return;
	}

	printf("Username : %s\n", newuser.data.name);
	printf("Coin : %d\n", newuser.data.coin);
	printf("USER REGISTER SUCCESS\n");

	return;
}

void U_add_node(UNODE* ulist, UNODE newuser) {
	UNODE* temp = ulist->next;
	UNODE* newnode = (UNODE*)malloc(sizeof(UNODE));

	if (newnode == NULL)
		return;

	newnode->data = newuser.data;

	if (temp == NULL) {
		newnode->next = NULL;
		ulist->next = newnode;
		
		return;
	}

	while (temp->next != NULL)
		temp = temp->next;

	newnode->next = NULL;
	temp->next = newnode;
	

	return;
}

void U_user_list(UNODE* ulist) {
	UNODE* temp = ulist->next;
	int num = 1;

	if (temp == NULL) {
		printf("There is no current user\n");
		return;
	}

	for (; temp != NULL; num++) {
		if (num < 10)
			printf("%d. Name : %s\n%3cCoin : %d\n", num, temp->data.name, ' ', temp->data.coin);
		else
			printf("%d. Name : %s\n%4cCoin : %d\n", num, temp->data.name, ' ', temp->data.coin);

		temp = temp->next;
	}

	return;
}

void U_get_user(UNODE* ulist, TNODE* tlist) {
	UNODE* temp = ulist;

	if (ulist->next == NULL) {
		printf("There is no current user\n");
		return;
	}

	temp = ulist->next;
	temp = find_user(temp, tlist->next->data.sender);
	if (temp == NULL) {
		printf("error\n");
		return;
	}

	printf("Current user name : %s\n", tlist->next->data.sender);
	printf("Amounts of coin : %d\n", temp->data.coin);

	return;
}

void U_ch_user(UNODE* ulist, TNODE* tlist) {
	char name[21] = "\0";
	UNODE* temp = ulist->next;

	if (temp == NULL) {
		printf("There is no user\n");
		return;
	}

	printf("Enter use name >> ");
	scanf_s("%s", name, sizeof(name));

	temp = find_user(temp, name);
	if (temp == NULL) {
		printf("There is no user\n");
		while (getchar() != '\n');
		return;
	}

	strcpy_s(tlist->next->data.sender, sizeof(tlist->next->data.sender), name);
	tlist->next->data.coin = temp->data.coin;

	printf("USER CHANGE SUCCESS\n");
	
	return;
}

void U_quit(UNODE* ulist) {
	FILE* fp = NULL;
	UNODE* del = ulist, * after = ulist, * temp = ulist;
	errno_t err;

	err = fopen_s(&fp, "User.txt", "w");
	if (err != 0) {
		printf("Unable to make the file \"User.txt\"\n");
		return;
	}

	if (ulist->next == NULL) {
		free(ulist);
		return;
	}
	after = del->next;
	free(del);
	del = after;
	after = after->next;

	if (fp == 0) {
		printf("error\n");
		return;
	}
	fprintf(fp, "User\n");

	while (after != NULL) {
		fprintf(fp, "%s %d\n", del->data.name, del->data.coin);

		free(del);
		del = after;
		after = after->next;
	}

	fprintf(fp, "%s %d\n", del->data.name, del->data.coin);

	free(del);

	return;
}

UNODE* find_user(UNODE* utemp, char* standard) {

	while (utemp != NULL) {
		if (strcmp(utemp->data.name, standard) == 0)
			return utemp;

		utemp = utemp->next;
	}

	return utemp;
}
