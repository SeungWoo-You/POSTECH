#ifndef TRANS_H
#define TRANS_H

typedef struct {
	char sender[21];
	char receiver[21];
	int coin;
	int check;
} TRANS;

typedef struct trx {
	TRANS data;
	struct trx* next;
} TNODE;

#include "block.h"

void T_add_node(TNODE* tlist, TNODE trader);
void T_mktx(UNODE* ulist, TNODE* tlist);
void T_txlist(UNODE* ulist, TNODE* tlist);
void T_quit(TNODE* tlist);

#endif