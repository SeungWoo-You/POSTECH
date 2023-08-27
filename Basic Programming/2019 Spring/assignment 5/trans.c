#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

void T_add_node(TNODE* tlist, TNODE trader) {
	TNODE* temp = tlist->next;
	TNODE* newnode = (TNODE*)malloc(sizeof(TNODE));

	if (newnode == NULL)
		return;

	newnode->data = trader.data;

	if (temp == NULL) {
		newnode->next = NULL;
		tlist->next = newnode;
		return;
	}

	while (temp->next != NULL)
		temp = temp->next;

	newnode->next = NULL;
	temp->next = newnode;


	return;
}

void T_mktx(UNODE* ulist, TNODE* tlist) {
	char name[21] = "\0";
	UNODE* utemp = ulist;
	TNODE trader;

	if (ulist->next == NULL) {
		printf("There is no user\n");
		return;
	}

	printf("Receiver name: ");
	scanf_s("%s", name, sizeof(name));

	if (strcmp(name, tlist->next->data.sender) == 0) {
		printf("Sender and receiver are same\n");
		return;
	}

	utemp = ulist->next;
	utemp = find_user(utemp, name);
	if (utemp == NULL) {
		printf("There is no user\n");
		while (getchar() != '\n');
		return;
	}

	utemp = ulist->next;
	utemp = find_user(utemp, tlist->next->data.sender);
	if (utemp == NULL) {
		printf("error");
		return;
	}

	printf("Amounts of coin to transfer: ");
	scanf_s("%d", &(trader.data.coin));

	if (trader.data.coin < 0) {
		printf("You cannot trade coins less than 0\n");
	}
	if (utemp->data.coin < trader.data.coin) {
		printf("Your coin is not enough\n");
		return;
	}

	strcpy_s(trader.data.sender, sizeof(trader.data.sender), tlist->next->data.sender);
	strcpy_s(trader.data.receiver, sizeof(trader.data.receiver), name);
	trader.data.check = 0;

	T_add_node(tlist, trader);
	
	printf("TRANSACTION CREATION SUCCESS\n");

	return;
}

void T_txlist(UNODE* ulist, TNODE* tlist) {
	UNODE* utemp = ulist;
	TNODE* ttemp = tlist;

	if (ulist->next == NULL) {
		printf("There is no user\n");
		return;
	}
	utemp = ulist->next;

	if (tlist->next->next == NULL) {
		printf("There is no transaction\n");
		return;
	}
	ttemp = tlist->next->next;

	while (ttemp != NULL) {
		if (ttemp->data.check == 0) {
			printf("Sender : %s\n", ttemp->data.sender);
			printf("Receiver : %s\n", ttemp->data.receiver);
			printf("Transferred coin : %d\n\n", ttemp->data.coin);
		}
		ttemp = ttemp->next;
	}

	return;
}

void T_quit(TNODE* tlist) {
	FILE* fp = NULL;
	TNODE* del = tlist, * after = tlist, * temp = tlist;
	errno_t err;

	err = fopen_s(&fp, "Transaction.txt", "w");
	if (err != 0) {
		printf("Unable to make the file \"Transaction.txt\"\n");
		return;
	}

	if (tlist->next == NULL) {
		free(tlist);
		return;
	}
	after = del->next;
	free(del);
	del = after;
	after = after->next;
	free(del);
	del = after;
	after = after->next;

	if (fp == 0) {
		printf("error\n");
		return;
	}
	fprintf(fp, "Transaction\n");

	while (after != NULL) {
		if (del->data.check == 0) {
			fprintf(fp, "Sender %s\n", del->data.sender);
			fprintf(fp, "Receiver %s\n", del->data.receiver);
			fprintf(fp, "Coin %d\n", del->data.coin);
		}

		free(del);
		del = after;
		after = after->next;
	}

	if (del->data.check == 0) {
		fprintf(fp, "Sender %s\n", del->data.sender);
		fprintf(fp, "Receiver %s\n", del->data.receiver);
		fprintf(fp, "Coin %d\n", del->data.coin);
	}

	free(del);

	return;
}