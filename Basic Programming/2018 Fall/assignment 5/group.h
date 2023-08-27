#ifndef GROUP_H	// 헤더파일 중복 방지 
#define GROUP_H

typedef struct list{
	STUDENT data;	// 학생 정보 저장 
	struct list *next;
} NODE;

typedef struct {
	int *count;	// 배열을 위해 *count 선언 
	NODE **tail;	// 배열을 위해 **tail 선언. circular linked list이므로 tail의 정보를 저장하는 것이 편리 
} LIST;

void G_LoadData(LIST *slist, char *argv[]);	// 처음 시작 때 파일의 정보를 읽어와 출력 
void G_AddNode(LIST *slist, NODE node, int n);	// node 1개 추가 
NODE* G_Out(LIST *slist, NODE* temp, int nextgroup, int nextplayer);	// 게임 진행 시 두 학생의 위치 교체(한명은 밖에, 한명은 linked list안에 있으므로 list 밖으로 빼내고 그자리에 다른 한명을 넣는 작업과 같음) 
void G_DelNode(LIST *slist, int ID, int n);	// node 1개 제거 

#endif
