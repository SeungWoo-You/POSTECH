#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <math.h>

#define MAXLINE 68

typedef struct Station {
	char name[21];
	int posX;
	int posY;
} STATION;

typedef struct list {
	STATION data;	// �� ���� ���� 
	struct list* next;
	int count;
} NODE;

int Load_Station(NODE* station);
void Add_Node(NODE* station, NODE info, int pos);
void Copy_Data(NODE* to, NODE* from);
void Show(NODE* station);
void Add(NODE* station);
void Delete(NODE* station);
void Delete_Node(NODE* station, int pos);
void Move(NODE* station);
void Quit(NODE* station);
void line(int num, char c);


int main() {
	char command[10] = "";
	NODE* station = (NODE*)malloc(sizeof(NODE));

	station->count = 0; station->next = NULL;
	while (Load_Station(station))
		printf("\n");

	while (1) {
		printf("��ɾ �Է����ּ���. >> ");
		scanf("%s", command);
		while (getchar() != '\n');

		if (strcmp(command, "show") == 0)
			Show(station);

		else if (strcmp(command, "add") == 0)
			Add(station);

		else if (strcmp(command, "delete") == 0)
			Delete(station);

		else if (strcmp(command, "move") == 0)
			Move(station);

		else if (strcmp(command, "quit") == 0) {
			Quit(station);
			return 0;
		}

		else
			printf("[Error #2] ��ȿ���� ���� ��ɾ��Դϴ�.\n");

		printf("\n");
	}

	return 0;
}

int Load_Station(NODE* station) {
	FILE* file = NULL;
	char file_name[30] = "";
	NODE* temp = station, * new_node = NULL;

	printf("������ �� ����Ʈ�� ������ ���� �̸��� �Է����ּ���. >> ");
	scanf("%s", file_name); while (getchar() != '\n');
	
	file = fopen(file_name, "r");

	if (file == NULL) {
		printf("[Error #1] ���� �̸��� ��ȿ���� �ʽ��ϴ�.\n");

		return 1;
	}

	while (fscanf(file, "%s %d %d", station->data.name, &station->data.posX, &station->data.posY) != EOF) {
		new_node = (NODE*)malloc(sizeof(NODE));

		if (new_node == NULL) {
			printf("Allocation failed.\n");
			exit(0);
		}

		Copy_Data(new_node, station);

		station->count++;
		temp->next = new_node;
		temp = temp->next;
	}

	temp->next = NULL;

	return 0;
}

void Add_Node(NODE* station, NODE info, int pos) {
	NODE* prev_node = station, * now_node = prev_node->next;
	NODE* new_node = (NODE*)malloc(sizeof(NODE));
	int i = 0;

	if (new_node == NULL) {
		printf("Allocation failed.\n");
		exit(0);
	}

	new_node->next = NULL;
	for (i = 1; i < pos; i++)
		prev_node = prev_node->next;

	now_node = prev_node->next;
	prev_node->next = new_node;
	new_node->next = now_node;

	Copy_Data(new_node, &info);

	station->count++;

	return;
}

void Copy_Data(NODE* to, NODE* from) {
	strcpy(to->data.name, from->data.name);
	to->data.posX = from->data.posX;
	to->data.posY = from->data.posY;
}

void Show(NODE* station) {
	NODE* temp = station->next;
	int num = 1;

	if (temp == NULL) {
		printf("[Error #3] �� ����Ʈ�� ��� �ֽ��ϴ�.\n");

		return;
	}

	line(MAXLINE, '=');
	printf("  ��ȣ\t\t����\t\t\t\t\t��ġ(��ǥ)\t\n");
	line(MAXLINE, '-');

	for (temp; temp != NULL; temp = temp->next) {
		if (temp == station->next) {
			printf("%3d [����]\t%-20s\t\t\t(%d, %d)\t\n", num, temp->data.name, temp->data.posX, temp->data.posY);
			num++; continue;
		}

		else if (temp->next == NULL) {
			printf("%3d [����]\t%-20s\t\t\t(%d, %d)\t\n", num, temp->data.name, temp->data.posX, temp->data.posY);
			continue;
		}

		printf("%3d\t\t%-20s\t\t\t(%d, %d)\t\n", num, temp->data.name, temp->data.posX, temp->data.posY);
		num++;
	}

	line(MAXLINE, '=');
	printf("[total: %d stations]\n", station->count);

	return;
}

void Add(NODE* station) {
	int stn_num = 0, check = 0, error = 0;
	double dist = 0;
	NODE* temp = station->next, new_station;

	printf("+ �� ��°�� �� ������ �߰��ұ��? >> ");
	scanf("%d", &stn_num);
	printf("+ �߰��� �� ������ ������ �Է����ּ���. >> ");
	scanf("%s", new_station.data.name);
	printf("+ �߰��� �� ������ ��ġ ���� x ��ǥ�� �Է����ּ���. >> ");
	scanf("%d", &new_station.data.posX);
	printf("+ �߰��� �� ������ ��ġ ���� y ��ǥ�� �Է����ּ���. >> ");
	scanf("%d", &new_station.data.posY);

	while (temp != NULL) {
		if (strcmp(temp->data.name, new_station.data.name) == 0) {
			check = 1; break;
		}

		if (temp->data.posX == new_station.data.posX && temp->data.posY == new_station.data.posY) {
			check = 1; break;
		}

		dist = sqrt(pow(temp->data.posX - new_station.data.posX, 2) + pow(temp->data.posY - new_station.data.posY, 2));
		if (dist < 3) {
			check = 1; break;
		}

		temp = temp->next;
	}

	if (stn_num < 1 || stn_num >(station->count + 1)) {
		printf("[Error #4] %d ��°�� ��ġ�� �߰��� �� �����ϴ�.\n", stn_num);
		error++;
	}

	if (check == 1) {
		printf("[Error #5] ��ȿ���� ���� �� �����Դϴ�.\n");
		error++;
	}

	if (error != 0)
		return;

	Add_Node(station, new_station, stn_num);

	return;
}

void Delete(NODE* station) {
	int pos = -1, error = 0;

	printf("+ �� ��°�� �� ������ �����ұ��? >> ");
	scanf("%d", &pos);

	if (station->count == 0) {
		printf("[Error #7] �� ����Ʈ�� ��� �ֽ��ϴ�.\n");
		error++;
	}

	if (pos < 1 || pos > station->count) {
		printf("[Error #6] %d ��°�� �� ������ �������� �ʽ��ϴ�.\n", pos);
		error++;
	}

	if (error != 0)
		return;

	Delete_Node(station, pos);

	return;
}

void Delete_Node(NODE* station, int pos) {
	NODE* prev_node = station, * now_node = prev_node->next;
	int i;



	for (i = 1; i < pos; i++) {
		prev_node = prev_node->next;
	}

	now_node = prev_node->next;
	prev_node->next = now_node->next;
	
	station->count--;
	free(now_node);
	

	return;
}

void Move(NODE* station) {
	NODE* from_node_prev = station , * from_node = from_node_prev->next, * to_node_prev = station, * to_node = to_node_prev->next;
	int from_pos = -1, to_pos = -1, i;

	printf("+ �� ��°�� �� ������ �̵��ұ��? >> ");
	scanf("%d", &from_pos);

	printf("+ %d ��°�� �� ������ �� ��°�� �̵��ұ��? >> ", from_pos);
	scanf("%d", &to_pos);

	if (from_pos < 1 || from_pos > station->count || to_pos < 1 || to_pos > station->count) {
		printf("[Error #8] ��ȿ���� ���� �Է��Դϴ�.\n");
		return;
	}

	if (from_pos == to_pos)
		return;

	for (i = 1; i < from_pos; i++)
		from_node_prev = from_node_prev->next;

	from_node = from_node_prev->next;

	if (from_pos > to_pos)
		for (i = 1; i < to_pos; i++)
			to_node_prev = to_node_prev->next;
	
	else
		for (i = 0; i < to_pos; i++)
			to_node_prev = to_node_prev->next;
	
	to_node = to_node_prev->next;

	from_node_prev->next = from_node->next;
	from_node->next = to_node;
	to_node_prev->next = from_node;
	

	return;
}

void Quit(NODE* station) {
	FILE* file = NULL;
	NODE* now_node = station, * next_node = now_node->next;
	char file_name[30] = "";

	printf("+ ������ ���� �̸��� �Է����ּ���. >> ");
	scanf("%s", file_name);

	if (station->count == 0) {
		printf("[Error #9] �� ����Ʈ�� ������ ������ �����ϴ�.\n");
		return;
	}

	file = fopen(file_name, "w");

	free(now_node);
	while (next_node != NULL) {
		now_node = next_node;
		next_node = next_node->next;
		fprintf(file, "%s %d %d\n", now_node->data.name, now_node->data.posX, now_node->data.posY);
		free(now_node);
	}

	return;
}

void line(int num, char c) {
	int i;

	for (i = 0; i < num; i++)
		printf("%c", c);

	printf("\n");

	return;
}
