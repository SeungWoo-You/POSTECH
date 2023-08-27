#ifndef BLOCK_H
#define BLOCK_H

typedef struct {
	UNODE* blkmake;
	TNODE* trade;
	TNODE* trade2;
	TNODE* trade3;
} BLOCK;

typedef struct blk {
	BLOCK data;
	struct blk* next;
} BNODE;

void B_mk_gen_blk(BNODE* blist);
void B_add_node(BNODE* blist);
void B_blk_list(BNODE* blist);
void B_mk_blk(BNODE* blist);
void B_quit(BNODE* blist);

#endif