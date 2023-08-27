#pragma once

#define MLEN 15
#define CAPACITY 50
#define FNAME 31

typedef struct music {
	char title[MLEN];
	char artist[MLEN];
	float size;
	float prefer;
} MUSIC;

typedef struct node {
	int count;
	float storage;
	MUSIC data;
	struct node* next;
} NODE;

void show_fn(NODE* head);
void show_favorites_fn(NODE* head);
void add_fn(NODE* head);
void delete_fn(NODE* head);
void exit_fn(NODE* head);

void read_list(NODE* head);
void copy_data(NODE* to, MUSIC from);
void add_node(NODE* head, MUSIC info, int pos);
void remove_node(NODE* head, int pos);
void add_alphabetical(NODE* head, MUSIC info);