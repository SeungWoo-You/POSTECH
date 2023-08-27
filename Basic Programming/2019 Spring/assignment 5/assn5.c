#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

void show_menu();
void divide_line(int num);

int main() {
	char cmd[10] = "\0", help[] = "help", useradd[] = "useradd", userlist[] = "userlist", getUser[] = "getUser", chuser[] = "chuser", mkGenBLK[] = "mkGenBLK", mkTx[] = "mkTx", Txlist[] = "Txlist", mkBLK[] = "mkBLK", blklist[] = "blklist", quit[] = "quit";
	UNODE* ulist = (UNODE*)malloc(sizeof(UNODE));
	TNODE* tlist = (TNODE*)malloc(sizeof(TNODE));
	BNODE* blist = (BNODE*)malloc(sizeof(BNODE));

	if (ulist == NULL || tlist == NULL || blist == NULL)
		return -1;
	
	ulist->next = NULL;
	tlist->next = NULL;
	blist->next = NULL;

	blist->data.blkmake = ulist;
	blist->data.trade = tlist;
	blist->data.trade2 = NULL;
	blist->data.trade3 = NULL;
	show_menu();
	while (1) {
		printf("\n>> ");
		scanf_s("%s", cmd, sizeof(cmd));
		
		if (_stricmp(cmd, help) == 0) {
			show_menu();
			continue;
		}

		if (_stricmp(cmd, useradd) == 0) {
			U_user_add(ulist, tlist);
			continue;
		}

		if (_stricmp(cmd, userlist) == 0) {
			U_user_list(ulist);
			continue;
		}

		if (_stricmp(cmd, getUser) == 0) {
			U_get_user(ulist, tlist);
			continue;
		}

		if (_stricmp(cmd, chuser) == 0) {
			U_ch_user(ulist, tlist);
			continue;
		}

		if (_stricmp(cmd, mkGenBLK) == 0) {
			B_mk_gen_blk(blist);
			continue;
		}

		if (_stricmp(cmd, mkTx) == 0) {
			T_mktx(ulist, tlist);
			continue;
		}

		if (_stricmp(cmd, Txlist) == 0) {
			T_txlist(ulist, tlist);
			continue;
		}

		if (_stricmp(cmd, mkBLK) == 0) {
			B_mk_blk(blist);
			continue;
		}

		if (_stricmp(cmd, blklist) == 0) {
			B_blk_list(blist);
			continue;
		}

		if (_stricmp(cmd, quit) == 0) {
			B_quit(blist);
			T_quit(tlist);
			U_quit(ulist);
			return 0;
		}

		else {
			while (getchar() != '\n');
			printf("worng command!\n");
			continue;
		}
	}
}

void show_menu() {
	divide_line(29);
	printf(" * Mini Bitcoin Simulation * \n");
	divide_line(29);
	printf("help: 명령어 목록 보기\n");
	printf("useradd: 사용자 등록\n");
	printf("userlist: 등록된 모든 사용자 보기\n");
	printf("getUser: 현재 사용자 확인\n");
	printf("chuser: 사용자 변경\n");
	printf("mkGenBLK: Genesis Block 생성\n");
	printf("mkTx: Transaction 생성\n");
	printf("Txlist: Transaction 목록 보기\n");
	printf("mkBLK: Block 생성\n");
	printf("blklist: BlockChain(거래 내역) 확인\n");
	printf("quit: 프로그램 종료\n");
	divide_line(29);
}

void divide_line(int num) {
	int i;

	for (i = 0; i < num; i++)
		printf("-");
	printf("\n");

	return;
}