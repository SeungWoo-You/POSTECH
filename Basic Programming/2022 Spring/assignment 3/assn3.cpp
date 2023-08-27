#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MAX_SIZE 10  // �ִ� �迭 ũ��
#define MIN_TILE 1  // �ּ� Ÿ�� ��ȣ
#define MAX_TILE 8  // �ִ� Ÿ�� ��ȣ
#define LINE 24  // �� �ٿ� ���� ���� ��

enum ColorType {  // �������� �̿��� �������~(���� ����)
	BLACK,  	//0
	DarkBLUE,	//1
	DarkGreen,	//2
	DarkSkyBlue,    //3
	DarkRed,  	//4
	DarkPurple,	//5
	DarkYellow,	//6
	GRAY,		//7
	DarkGray,	//8
	BLUE,		//9
	GREEN,		//10
	SkyBlue,	//11
	RED,		//12
	PURPLE,		//13
	YELLOW,		//14
	WHITE		//15
};

int make_board(int board[MAX_SIZE][MAX_SIZE], int* row, int* col);  // board.txt ���� �а� ������ ������ ����
void print_board(int board[MAX_SIZE][MAX_SIZE], int row, int col);  // ������ ���
int remove_tiles(int board[MAX_SIZE][MAX_SIZE], int row, int col);  // Ÿ�� ����
void update_board(int board[MAX_SIZE][MAX_SIZE], int row, int col);  // Ÿ�� ������Ʈ
void Copy_Board(int from_board[MAX_SIZE][MAX_SIZE], int to_board[MAX_SIZE][MAX_SIZE], int row, int col); // ������ ����
int Check_Square(int board[MAX_SIZE][MAX_SIZE], int row, int col);  // Ÿ�� ���� �� �ִ���(2X2 ���簢������) Ȯ��
int Delete_Tile(int board[MAX_SIZE][MAX_SIZE], int row, int col); // Ÿ�� �����(0���� �ٲٰ� ������ Ƚ�� ī��Ʈ)
void Print_Result(int change_count);  // ���� ��� ���(�ܼ�)
void Print_Result(int change_count, FILE* file); // ���� ��� ���(����)
void End_Board(int board[MAX_SIZE][MAX_SIZE], int row, int col, int change_count);  // result.txt ���� ����
void Text_Color(int colorNum);  // Ÿ�� ���� �� ����(���� ����)
void line(int num);  // �� �߱�(�ܼ�)
void line(int num, FILE* file);  // �� �߱�(����)
void enter();  // ���� �Է� �ޱ�
void clear();  // ȭ�� �����
void swap(int* p, int* q);  // �� Ÿ�� ��ġ ��ȯ

int main() {
	int row = MAX_SIZE, col = MAX_SIZE, i = 0, j = 0, change_count = 0, temp_count = 0;
	int board[MAX_SIZE][MAX_SIZE] = { 0 };

	switch (make_board(board, &row, &col)) {  // ���� �˻�(�ܿ��� ���� ����� MAX_SIZE���� ũ�ٰų� �ϴ� ���� �پ��� ���� ó�� Ȯ�� ����)
	case 100:  // board.txt ���� X
		printf("Error code : 100\n");
		return 0;
	case 200:  // range(1 ~ 8) ���� ���� ����
		printf("Error code : 200\n");
		return 0;
	case 300:  // �ʿ��� ���ڸ�ŭ ������� ���� ��
		printf("Error code : 300\n");
		return 0;
	case 1:
		break;
	}

	while (1) {  // ���� ����
		print_board(board, row, col);
		temp_count = remove_tiles(board, row, col);

		if (temp_count == 0) {	// board�� �ٲ��� �ʾ��� �� -> ���� ���� 
			Print_Result(change_count);
			break;
		}

		change_count += temp_count;

		print_board(board, row, col);
		update_board(board, row, col);
	}

	End_Board(board, row, col, change_count);

	return 0;
}
	
int make_board(int board[MAX_SIZE][MAX_SIZE], int* row, int* col) {
	FILE* file_board;
	int i = 0, j = 0, check; // check: ���� Ȯ�� ��

	file_board = fopen("board.txt", "r");
	if (file_board == NULL) {  // ���� ���� ��
		printf("\aFile opening error : The file 'board.txt' does not exist.\n"); // \a: �˸���(�����) ȿ��
		return 100;
	}

	fscanf(file_board, "%d%d", row, col);

	for (i = 0; i < *row; i++) {  // board data �Է�
		for (j = 0; j < *col; j++) {
			check = fscanf(file_board, "%d", &board[i][j]);
			if (check != 1) {	// check�� ���� �ִٸ�(��ĵ�� ���������� �ߴٸ�) 1 ��ȯ -> 1�� �ƴ϶�� �ʿ��� ���ں��� ���� ������ ���ڰ� �ԷµǾ� ����(�Ǵ� ������ ������)
				printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
				return 300;
			}

			if ((board[i][j] < MIN_TILE) || (board[i][j] > MAX_TILE)) {  // ���忡 ���� ���� ������ ����
				printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : %d to %d)\n", MIN_TILE, MAX_TILE);
				return 200;
			}
		}
	}

	check = fscanf(file_board, "%d", &board[i][j]);  // �Է� ���� �� ���� �ִ��� �� �� �� �˻� 
	if (check == 1) {	// check�� 1�̸�(��ĵ�� ���������� �ߴٸ�) �ʿ��� ���ں��� ���� ������ ���ڰ� �ԷµǾ� ���� 
		printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
		return 300;
	}

	fclose(file_board);

	return 1;	// �������� �Ϸ��ϸ� 1�� ��ȯ
}

void print_board(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	int i, j;

	clear();

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			switch (board[i][j]) {  // ���ڿ� �´� ������ ����
			case 0:
				Text_Color(DarkGray); break;
			case 1:
				Text_Color(DarkBLUE); break;
			case 2:
				Text_Color(DarkGreen); break;
			case 3:
				Text_Color(DarkSkyBlue); break;
			case 4:
				Text_Color(DarkRed); break;
			case 5:
				Text_Color(DarkPurple); break;
			case 6:
				Text_Color(DarkYellow); break;
			case 7:
				Text_Color(YELLOW); break;
			case 8:
				Text_Color(RED); break;
			}

			printf("%d ", board[i][j]);
			Text_Color(WHITE);
		}   // �������. switch case �κ� ������ ��

		printf("\n");
	}

	enter();
}

int remove_tiles(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	int sub_board[MAX_SIZE][MAX_SIZE] = { 0 };
	int i = 0, j = 0;
	int col_check = j, change_tile = 0;

	Copy_Board(board, sub_board, row, col);  // ���� ���� ���� -> ���� ����� ������ ������, ���� ����� �˻�(���� Ȯ��)������ ��� -> �˻簡 ������ ���� ����� �츮�� ���ϴ� ���� ���¸� ����

	for (i = 0; i < row - 1;) {  // i�� �ݺ���, j�� �˻������ ��� 
		col_check = Check_Square(sub_board, i, j);  // ���� ��ġ�� �������� ���簢���� �Ǿ��ִ��� Ȯ��

		if (j != col_check)  // ���簢���� �ƴ϶�� ���� ��ġ�� �ű��(�� �˻��� �ʿ� X)
			j = col_check;

		else {  // ���簢���̸� Ÿ�� ����� �˻� ��ġ �ű��
			change_tile += Delete_Tile(board, i, j);
			j++;
		}

		if (j >= col - 1) {
			j = 0; i++;
		}
	}

	return change_tile;  // �ٲ�� ������ 0 ��ȯ -> ���� �������� �̿�
}

void update_board(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	int i, j, temp;  // temp: �ű� ���ڰ� �ִ� ��ġ �˷��ִ� �뵵

	for (j = 0; j < col; j ++) {  // ���η� ���� �ø��� �˰���
		temp = row - 1;

		for (i = row - 1; i >= 0; i--) {  // ���η� 0�� ���� �ø�. �ٴڿ��� ���� �˻�. i�� �������ѳ��� temp�� ������. i�� 0�� ����Ű��, temp�� �Űܾ� �� ���ڸ� ã���� i�� swap

			if (board[i][j] != 0) {  // �ٴ� ���� 0�� �ƴϸ� �ø� �ʿ� X -> �Ѿ
				temp--; continue;
			}

			while (temp >= 0) {  // temp �̵�
				if (board[temp][j] == 0)  // temp�� ���ڸ� ã�ƾ� ��
					temp--;
				
				else {
					swap(&board[i][j], &board[temp][j]);
					temp--; break;
				}
			}

			if (temp < 0)  // temp�� �ؿ������� ������� ��� ���ڸ� ����Ű�⿡, temp�� 0���� ������ �߰� �˻� �ǹ� X
				break;
		}
	}
}

void Copy_Board(int from_board[MAX_SIZE][MAX_SIZE], int to_board[MAX_SIZE][MAX_SIZE], int row, int col) {
	int i = 0, j = 0;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			to_board[i][j] = from_board[i][j];
		}
	}
}

int Check_Square(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	if (board[row][col] == 0) // ���� ��ġ Ÿ���� 0�̸� ���� ������ -> �˻� X
		return col + 1;

	if (board[row][col] != board[row + 1][col] || board[row][col] != board[row][col + 1])  // ���� ��ġ Ÿ�ϰ� �Ʒ� & �������� �ٸ� ���̸� ���������� 1ĭ �̵��ؼ� �˻�
		return col + 1;

	if (board[row][col] != board[row + 1][col + 1]) // ���� ��ġ Ÿ�ϰ� �밢 ��ġ�� �ٸ� ���̸� ������ 2ĭ �̵��ؼ� �˻�. 1ĭ �̵��غ��� ������ �ٸ� �� Ȯ���̹Ƿ� �ٷ� 2ĭ �̵�
		return col + 2;

	return col;
}

int Delete_Tile(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	int i, j, count = 0;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			if (board[row + i][col + j] != 0) { // Ÿ���� ����鼭 ���� Ÿ�� ���� ī��Ʈ
				board[row + i][col + j] = 0;
				count++;
			}
		}
	}

	return count;
}

void Print_Result(int change_count) {
	line(LINE);
	printf("Total removed tiles: %d\n", change_count);
	line(LINE);

	system("pause");
}

void Print_Result(int change_count, FILE* file) {
	line(LINE, file);
	fprintf(file, "Total removed tiles: %d\n", change_count);
	line(LINE, file);
}

void End_Board(int board[MAX_SIZE][MAX_SIZE], int row, int col, int change_count) {
	FILE* end_file;
	int i, j;

	end_file = fopen("result.txt", "w");

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			fprintf(end_file, "%d ", board[i][j]);
		}
		fprintf(end_file, "\n");
	}

	fprintf(end_file, "\n");
	Print_Result(change_count, end_file);

	fclose(end_file);
}

void Text_Color(int color_num) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_num);
}

void line(int num) {
	int i = 0;
	
	for (i = 0; i < num; i++) {
		printf("=");
	}

	printf("\n");
}

void line(int num, FILE* file) {
	int i = 0;

	for (i = 0; i < num; i++) {
		fprintf(file, "=");
	}

	fprintf(file, "\n");
}

void enter() {
	while (getchar() != '\n');
}

void clear() {
	system("cls");
}

void swap(int* p, int* q) {
	int temp;

	temp = *p;
	*p = *q;
	*q = temp;
}