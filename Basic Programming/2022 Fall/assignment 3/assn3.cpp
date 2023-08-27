#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define OVERLOOP 12345678
#define NAME 21
#define RED 91
#define GREEN 92
#define YELLOW 93
#define BLUE 94
#define PURPLE 95

void generate_ladder();
void save_ladder(char filename[], int** board, int player, int height, int cross);
void free_ladder(int** board, int row);
int** load_ladder(char filename[], int* player, int* height);
void set_vt_mode();
void set_color(int code);
void reset_color();

int print_menu();
void get_info(int* player, int* height, int* cross, char name[]);
int set_cross(int valid, int cross);
int** alloc2D(int row, int col);
int random(int start, int end);
void play();
int find_path(int** ladder_board, int width, int height, int choice, int enter);
void print_ladder(int** board, int row, int col);
int pick_color(int num);


int main() {
	int menu_choice = 0;

	set_vt_mode();
	srand(time(NULL));

	do {
		menu_choice = print_menu();
		switch (menu_choice) {
		case 1:
			generate_ladder();
			break;
		case 2:
			play();
		case 3:
			break;
		default:
			printf("Wrong input!\n");
			menu_choice = 0;
		}
	} while (menu_choice != 3);
	
	return 0;
}

void generate_ladder() {
	int** ladder_board = NULL;
	int player = 0, height = 0, cross = 0;
	int loop = 0, cross_count = 0, row = -1, col = -1;
	char filename[NAME] = "";
	
	get_info(&player, &height, &cross, filename);
	ladder_board = alloc2D(height, player);

	while (cross_count < cross) {
		row = random(1, height - 2);
		col = random(0, player - 2);
		if (ladder_board[row][col] != 0) {
			loop++;
			continue;
		}
		ladder_board[row][col] = 1;
		if (col != 0)
			ladder_board[row][col - 1] = -1;
		ladder_board[row][col + 1] = -1;
		cross_count++;
	}

	if (loop >= OVERLOOP && cross_count != cross) {
		printf("Failed to create cross ladder.\n");
		printf("ERROR: overlooped.\n");
		free_ladder(ladder_board, height);
		return;
	}

	save_ladder(filename, ladder_board, player, height, cross);
	free_ladder(ladder_board, height);
	return;
}

void save_ladder(char filename[], int** board, int player, int height, int cross) {
	FILE* file = NULL;
	int i, j;

	file = fopen(filename, "w");
	if (file == NULL) {
		printf("Failed to create file.\n");
		printf("ERROR: fopen failed.\n");
		return;
	}

	fprintf(file, "%d %d %d\n", player, height, cross);
	for (i = 0; i < height; i++) {
		for (j = 0; j < player; j++) {
			if (board[i][j] == 1)
				fprintf(file, "%d %d\n", i, j + 1);
		}
	}

	fclose(file);
	return;
}

void free_ladder(int** board, int row) {
	int i;

	for (i = 0; i < row; i++)
		free(board[i]);
	free(board);
}

int** load_ladder(char filename[], int* player, int* height) {
	FILE* file = NULL;
	int** ladder_board = NULL;
	int row = -1, col = -1;

	printf("파일 이름: ");
	scanf("%s", filename);
	file = fopen(filename, "r");
	if (file == NULL) {
		printf("Failed to read file.\n");
		printf("ERROR: fopen failed.\n");
		return NULL;
	}

	fscanf(file, "%d %d %*d", player, height);
	ladder_board = alloc2D(*height, *player * 2 - 1);
	while (!feof(file)) {
		fscanf(file, "%d %d", &row, &col);
		ladder_board[row][col * 2 - 1] = -1;
	}
	for (row = 0; row < *height; row++) {
		for (col = 0; col < *player * 2 - 1; col += 2) {
			ladder_board[row][col] = -1;
			printf("%d ", ladder_board[row][col]);
		}
		printf("\n");
	}
	return ladder_board;
}

void set_vt_mode() {
	DWORD l_mode;
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &l_mode);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), l_mode | 0x0004 | 0x0008);
}

void set_color(int code) {
	printf("\033[%dm", code);
}

void reset_color() {
	printf("\033[0m");
}

int print_menu() {
	int select = 0;

	printf("사다리 게임\n");
	printf("================================\n");
	printf("1. 사다리 보드 생성\n");
	printf("2. 사다리 타기 시작\n");
	printf("3. 종료\n");
	printf("================================\n");
	printf("선택: ");
	scanf("%d", &select);
	while (getchar() != '\n');
	return select;
}

void get_info(int* player, int* height, int* cross, char filename[]) {
	int max_cross = 0;

	printf("참여 인원수: ");
	scanf("%d", player);
	while (getchar() != '\n');

	printf("사다리 높이: ");
	scanf("%d", height);
	while (getchar() != '\n');

	printf("가로줄 개수: ");
	scanf("%d", cross);
	while (getchar() != '\n');
	max_cross = (*height - 2) * (*player - 1);
	*cross = set_cross(max_cross, *cross);

	printf("파일이름: ");
	scanf("%s", filename);
	while (getchar() != '\n');
	return;
}

int set_cross(int valid, int cross) {
	if (cross < 0)
		return 0;
	else if (cross >= (int)(valid * 0.4))
		return (int)(valid * 0.4);
	else
		return cross;
}

int** alloc2D(int row, int col) {
	int i;
	int** board = NULL;

	board = (int**)calloc(row, sizeof(int*));
	if (board == NULL) {
		printf("Cannnot create ladder board.\n");
		printf("ERROR: out of memory.\n");
		exit(0);
	}
	
	for (i = 0; i < row; i++) {
		board[i] = (int*)calloc(col, sizeof(int));
		if (board[i] == NULL) {
			printf("Cannnot create ladder board.\n");
			printf("ERROR: out of memory.\n");
			exit(0);
		}
	}
	return board;
}

int random(int start, int end) {
	return rand() % (end - start + 1) + start;
}

void play() {
	char filename[NAME] = "";
	int** ladder_board = NULL, * result = NULL;
	int player = 0, height = 0, width = 0;
	int choice = -2, dst = 0;
	
	ladder_board = load_ladder(filename, &player, &height);
	width = player * 2 - 1;
	system("cls");
	print_ladder(ladder_board, height, width);

	while (1) {
		printf(">>");
		scanf("%d", &choice);
		while (getchar() != '\n');
		if (choice > player || choice < -1) {
			printf("Wrong input!\n");
			continue;
		}

		if (choice == 0)
			break;
		else if (choice == -1) {
			result = (int*)malloc(player * sizeof(int));
			if (result == NULL) {
				printf("Cannnot create ladder board.\n");
				printf("ERROR: out of memory.\n");
				exit(0);
			}

			for (choice = 1; choice <= player; choice++)
				result[choice - 1] = find_path(ladder_board, width, height, choice, 0);

			printf("\n");
			for (choice = 1; choice <= player; choice++)
				printf("%d -> %c\n", choice, 'A' + result[choice - 1] / 2);
			printf("\n");
			free(result);
			continue;
		}
		else {
			dst = find_path(ladder_board, width, height, choice, 1);
			printf("\n");
			printf("%d -> %c\n", choice, 'A' + dst / 2);
			printf("\n");
			continue;
		}
	}
	system("cls");
	free_ladder(ladder_board, height);
	return;
}

int find_path(int** ladder_board, int width, int height, int choice, int enter) {
	int i = height - 1, j = 2 * (choice - 1);

	while (i >= 0) {
		ladder_board[i][j] = choice;
		system("cls");
		printf("%d %d\n", i, j);
		print_ladder(ladder_board, height, width);

		if (j == width - 1) {
			if (ladder_board[i][j - 1] != 0 && ladder_board[i][j - 1] != choice)
				j = j - 1;
			else
				i = i - 1;
		}
		else if (j == 0) {
			if (ladder_board[i][j + 1] != 0 && ladder_board[i][j + 1] != choice)
				j = j + 1;
			else
				i = i - 1;
		}
		else {
			if (ladder_board[i][j - 1] != 0 && ladder_board[i][j - 1] != choice)
				j = j - 1;
			else if (ladder_board[i][j + 1] != 0 && ladder_board[i][j + 1] != choice)
				j = j + 1;
			else
				i = i - 1;
		}

		if (enter == 1) {
			scanf("%*[^\n]s");
			while (getchar() != '\n');
		}
	}
	return j;
}

void print_ladder(int** board, int row, int col) {
	int i, j;
	int src = 1;
	char dst = 'A';

	for (j = 0; j < col; j += 2) {
		printf("%c   ", dst);
		dst++;
	}
	printf("\n");

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (board[i][j] > 0)
				set_color(pick_color(board[i][j]));
			if (board[i][j] == 0)
				printf("   ");
			else if (j % 2 == 0)
				printf("+");
			else
				printf("---");
			reset_color();
		}
		printf("\n");
	}

	for (j = 0; j < col; j += 2) {
		printf("%d   ", src);
		src++;
	}
	printf("\n");
}

int pick_color(int num) {
	switch (num % 5) {
	case 1:
		return RED;
	case 2:
		return GREEN;
	case 3:
		return YELLOW;
	case 4:
		return BLUE;
	case 0:
		return PURPLE;
	}
	return 0;
}