#ifndef USER_H
#define USER_H

typedef struct {
	char name[21];
	int coin;
} USER;

typedef struct list {
	USER data;
	struct list* next;
} UNODE;

#include "trans.h"

void U_user_add(UNODE* ulist, TNODE* tlist);
void U_add_node(UNODE* ulist, UNODE newuser);
void U_user_list(UNODE* ulist);
void U_get_user(UNODE* ulist, TNODE* tlist);
void U_ch_user(UNODE* ulist, TNODE* tlist);
void U_quit(UNODE* ulist);
UNODE* find_user(UNODE* utemp, char* standard);

#endif