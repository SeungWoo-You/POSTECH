#ifndef GROUP_H	// ������� �ߺ� ���� 
#define GROUP_H

typedef struct list{
	STUDENT data;	// �л� ���� ���� 
	struct list *next;
} NODE;

typedef struct {
	int *count;	// �迭�� ���� *count ���� 
	NODE **tail;	// �迭�� ���� **tail ����. circular linked list�̹Ƿ� tail�� ������ �����ϴ� ���� �� 
} LIST;

void G_LoadData(LIST *slist, char *argv[]);	// ó�� ���� �� ������ ������ �о�� ��� 
void G_AddNode(LIST *slist, NODE node, int n);	// node 1�� �߰� 
NODE* G_Out(LIST *slist, NODE* temp, int nextgroup, int nextplayer);	// ���� ���� �� �� �л��� ��ġ ��ü(�Ѹ��� �ۿ�, �Ѹ��� linked list�ȿ� �����Ƿ� list ������ ������ ���ڸ��� �ٸ� �Ѹ��� �ִ� �۾��� ����) 
void G_DelNode(LIST *slist, int ID, int n);	// node 1�� ���� 

#endif
