#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

void B_mk_gen_blk(BNODE* blist) {
	BNODE* btemp = blist;
	TNODE* tlist = blist->data.trade, * ttemp = tlist;
	UNODE* ulist = blist->data.blkmake, * utemp = ulist;
	
	if (blist->next != NULL) {
		printf("Genesis block has been already created\n");
		return;
	}
	
	if (ulist->next == NULL) {
		printf("There is no user\n");
		return;
	}
	utemp = ulist->next;
	ttemp = tlist->next;
	
	B_add_node(blist);
	btemp = blist->next;

	if (btemp == NULL) {
		printf("error\n");
		return;
	}

	utemp = find_user(utemp, ttemp->data.sender);

	utemp->data.coin += 100;
	ttemp->data.coin = 100;
	btemp->data.blkmake = utemp;

	printf("GENESIS BLOCK CREATION SUCCESS\n");

	return;
}

void B_add_node(BNODE* blist) {
	BNODE* temp = blist->next;
	BNODE* newnode = (BNODE*)malloc(sizeof(BNODE));

	if (newnode == NULL)
		return;
	
	newnode->data.blkmake = NULL;
	newnode->data.trade = NULL;
	newnode->data.trade2 = NULL;
	newnode->data.trade3 = NULL;

	if (temp == NULL) {
		newnode->next = NULL;
		blist->next = newnode;
		return;
	}

	while (temp->next != NULL)
		temp = temp->next;

	newnode->next = NULL;
	temp->next = newnode;


	return;
}

void B_blk_list(BNODE* blist) {
	BNODE* btemp = blist;
	TNODE* tlist = blist->data.trade, * ttemp = tlist;
	int i, count = 0, check = 1;

	if (blist->next == NULL) {
		printf("There is no block\n");
		return;
	}

	while (btemp->next != NULL) {
		btemp = btemp->next;
		count++;
	}

	while (1) {
		ttemp = btemp->data.trade;
		check = 1;
		if (count == 1) {
			printf("======Genesis Block======\n");
			printf("***Special Transaction***\n");
			printf("Sender : Nothing\n");
			printf("Receiver : %s\n", btemp->data.blkmake->data.name);
			printf("Transferred coin : 100\n\n");
			return;
		}

		printf("==========Block==========\n");
		printf("***Special Transaction***\n");
		printf("Sender : Nothing\n");
		printf("Receiver : %s\n", btemp->data.blkmake->data.name);
		printf("Transferred coin : 100\n\n");

		while (ttemp != NULL) {
			printf("***Transaction***\n");
			printf("Sender : %s\n", ttemp->data.sender);
			printf("Receiver : %s\n", ttemp->data.receiver);
			printf("Transferred coin : %d\n\n", ttemp->data.coin);

			if (check == 1)
				ttemp = btemp->data.trade2;
			if (check == 2)
				ttemp = btemp->data.trade3;
			if (check == 3)
				break;
			check++;
		}

		btemp = blist->next;
		count--;
		for (i = 0; i < count - 1; i++)
			btemp = btemp->next;

		printf("\n");
	}
}

void B_mk_blk(BNODE* blist) {
	BNODE* btemp = blist;
	TNODE* tlist = blist->data.trade, * ttemp = tlist;
	UNODE* ulist = blist->data.blkmake, * utemp = ulist, * mkr = ulist;
	int count = 0;

	if (ulist->next == NULL) {
		printf("There is no user\n");
		return;
	}
	utemp = utemp->next;
	mkr = mkr->next;

	if (blist->next == NULL) {
		printf("Create genesis block first\n");
		return;
	}
	ttemp = ttemp->next;

	if (ttemp->next == NULL) {
		printf("There is not enough Transaction\n");
		return;
	}
	ttemp = ttemp->next;

	while (ttemp != NULL && count < 3) {
		if (ttemp->data.check != 0) {
			ttemp = ttemp->next;
			continue;
		}

		utemp = find_user(utemp, ttemp->data.sender);
		if (utemp == NULL) {
			printf("error\n");
			return;
		}

		if (utemp->data.coin < ttemp->data.coin) {
			ttemp = ttemp->next;
			utemp = ulist->next;
			continue;
		}

		else {
			if (count == 0) {
				B_add_node(blist);
				if (blist->next == NULL) {
					printf("error\n");
					return;
				}
			}
			btemp = blist->next;

			while (btemp->next != NULL)
				btemp = btemp->next;

			mkr = ulist->next;
			mkr = find_user(mkr, tlist->next->data.sender);
			if (mkr == NULL) {
				printf("error\n");
				return;
			}
			if (count == 0)
				mkr->data.coin += 100;
			btemp->data.blkmake = mkr;
			
			if (count == 0)
				btemp->data.trade = ttemp;
			if (count == 1)
				btemp->data.trade2 = ttemp;
			if (count == 2)
				btemp->data.trade3 = ttemp;

			utemp->data.coin -= ttemp->data.coin;
			utemp = ulist->next;
			utemp = find_user(utemp, ttemp->data.receiver);
			if (utemp == NULL) {
				printf("error\n");
				return;
			}
			utemp->data.coin += ttemp->data.coin;
			utemp = ulist->next;

			ttemp->data.check = 1;
			ttemp = ttemp->next;
			count++;
		}
	}

	if (count == 0) {
		printf("There is not enough Transaction\n");
		return;
	}

	printf("BLOCK CREATION SUCCESS\n");

	return;
}

void B_quit(BNODE* blist) {
	FILE* blkfp = NULL;
	BNODE* del = blist, * pre = blist, * temp = blist;
	TNODE* ttemp = blist->data.trade;
	errno_t err;
	int count = 0, check = 1;

	err = fopen_s(&blkfp, "BlockChain.txt", "w");
	if (err != 0) {
		printf("Unable to make the file \"BlockChain.txt\"\n");
		return;
	}

	if (blist->next == NULL) {
		free(blist);
		return;
	}

	while (del->next != NULL) {
		pre = del;
		del = del->next;
		count++;
	}

	if (blkfp == 0) {
		printf("error\n");
		return;
	}

	while (count > 0) {
		ttemp = del->data.trade;

		fprintf(blkfp, "Block\n");
		fprintf(blkfp, "Sender Nothing\n");
		fprintf(blkfp, "Receiver %s\n", del->data.blkmake->data.name);
		fprintf(blkfp, "Coin 100\n");

		check = 1;
		while (ttemp != NULL) {
			fprintf(blkfp, "Sender %s\n", ttemp->data.sender);
			fprintf(blkfp, "Receiver %s\n", ttemp->data.receiver);
			fprintf(blkfp, "Coin %d\n", ttemp->data.coin);
			if (check == 1)
				ttemp = del->data.trade2;
			if (check == 2)
				ttemp = del->data.trade3;
			if (check == 3)
				break;
			check++;
		}

		pre->next = NULL;
		free(del);
		del = pre;
		count--;

		pre = blist;
		if (count > 0) {
			while (pre->next->next != NULL)
				pre = pre->next;
		}
	}

	free(blist);

	return;
}