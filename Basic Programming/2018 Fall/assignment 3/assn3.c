#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MaxArr 10
#define GRY "\x1b[90m"	// ���󺯰� gray 
#define ORG "\x1b[91m"	// ���󺯰� orange 
#define GRN "\x1b[92m"	// ���󺯰� green
#define YEL "\x1b[93m"	// ���󺯰� yellow
#define BLU "\x1b[94m"	// ���󺯰� blue
#define PUP "\x1b[95m"	// ���󺯰� purple
#define SKY "\x1b[96m"	// ���󺯰� skyblue 
#define WHI "\x1b[97m"	// ���󺯰� white 
#define RES "\x1b[0m"	// ���� reset

int ReadBoard(int board[MaxArr][MaxArr], int* row, int* col);
void PrintBoard(int board[MaxArr][MaxArr], int row, int col);
int RemoveChunk(int board[MaxArr][MaxArr], int row, int col);
void UpdateBoard(int board[MaxArr][MaxArr], int row, int col);
void SubstBoard(int Arr[MaxArr][MaxArr], int Brr[MaxArr][MaxArr], int row, int col);	// Arr�� ���� Brr�� ������ substitution 
void ZeroUp(int board[MaxArr][MaxArr], int i, int j);	// board�� ǥ�õ� 0�� ���� �ø��� �Լ�
void EndBoard(int board[MaxArr][MaxArr], int row, int col);	// result.txt ����� �Լ�
void FillBlank(int board[MaxArr][MaxArr], int row, int col);
void clear();	// ȭ�� clear
void enter();	// Enter�� �Է¹޴� �Լ� 
int ColCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col);	// column�� 3���̻� ���� ���ڰ� �����Ǿ� �ִ��� Ȯ��
int RowCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col);	// row�� 3���̻� ���� ���ڰ� �����Ǿ� �ִ��� Ȯ�� 
void swap(int *p, int *q);

int main() {
	int row, col, i = 0, j = 0;
	int board[MaxArr][MaxArr] = {0};
	
	srand(time(NULL));

	switch (ReadBoard(board, &row, &col)) {
		case 100 :	// board.txt ���� x 
			printf("Error code : 100\n");
			return 0;
		case 200 :	// range(1 ~ 7) ���� ���� ����
			printf("Error code : 200\n");
			return 0;
		case 300 :	// �ʿ��� ���ڸ�ŭ ������� ���� ��
			printf("Error code : 300\n");
			return 0;
		case 1 :
			break;
	}
	clear();
	PrintBoard(board, row, col);	
	enter(); clear();
	RemoveChunk(board, row, col);
	
	while (1) {
		PrintBoard(board, row, col);
		enter(); clear();
		UpdateBoard(board, row, col);
		PrintBoard(board, row, col);
		enter(); clear();
		FillBlank(board, row, col);
		PrintBoard(board, row, col);
	
		if (RemoveChunk(board, row, col)) {	// board�� ���̻� �ٲ��� ���� �� ����� 
			printf("No more patterns to remove.\n");
			printf("Press Enter to end the game...\n");
			while (getchar() != '\n');
			break;
		}
		enter(); clear();
	}
	
	EndBoard(board, row, col);
	
	return 0;
}

int ReadBoard(int board[MaxArr][MaxArr], int* row, int* col) {
	FILE* flboard;
	int i = 0, j = 0, check;
	
	flboard = fopen("board.txt", "r");
	if (flboard == NULL) {
		printf("\aFile opening error : The file 'board.txt' does not exist.\n");
		return 100;
	}
	
	fscanf(flboard, "%d%d", row, col);
	
	for (i = 0; i < *row; i++) {
		for (j = 0; j < *col; j++) {
			check = fscanf(flboard, "%d", &board[i][j]);
			if (check != 1) {	// check�� ���� �ִٸ� 1�� ��ȯ�ϹǷ� 1�� �ƴ϶�� �ʿ��� ���ں��� ���� ������ ���ڰ� �ԷµǾ� ���� 
				printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
				return 300;
			}
			if ((board[i][j] < 1) || (board[i][j] > 7)) {
				printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : 1 to 7)\n");
				return 200;		
			}
		}
	}
	
	check = fscanf(flboard, "%d", &board[i][j]); // �������� �Է¹��� ��� ������ŭ �˻��ϹǷ� �Է� ���� �� ���� �ִ��� �� �� �� �˻� 
	if (check == 1) {	// check �� 1�� ��ȯ�Ѵٸ� �ʿ��� ���ں��� ���� ������ ���ڰ� �ԷµǾ� ���� 
		printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
		return 300;
	}
	
	fclose(flboard);
	
	return 1;	// �������� �����̸� 1�� ��ȯ
}

void PrintBoard(int board[MaxArr][MaxArr], int row, int col) {
	int i, j;
	
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			switch (board[i][j]) {
				case 0:
					printf(GRY); break;		// ���ڿ� �´� ������ ����
				case 1:
					printf(ORG); break;
				case 2:
					printf(GRN); break;
				case 3:
					printf(YEL); break;
				case 4:
					printf(BLU); break;
				case 5:
					printf(PUP); break;
				case 6:
					printf(SKY); break;
				case 7:
					printf(WHI); break;
			}

			printf("%d ", board[i][j]);
			printf(RES); 
		}
		printf("\n");
	}
}

int RemoveChunk(int board[MaxArr][MaxArr], int row, int col) {
	int RA[MaxArr][MaxArr] = { 0 }, CA[MaxArr][MaxArr] = { 0 };
	int i = 0, j = 0, ccheck = 0, rcheck = 0;	// ccheck, rcheck�� ���� column�� row�� ��ȭ�� �ִ��� check�ϴ� ���� 

	SubstBoard(RA, board, row, col);	// board�� RA�� ���� 
	SubstBoard(CA, board, row, col);	// board�� CA�� ���� 

	while (1) {	// column�� 3���̻� ������ ���ڰ� �ִ��� �˻��ؼ� 0���� �ٲ�
		if (i >= row)
			break;
		switch (ColCheck(CA, i, j, row, col)) {
		case 1: ++j; ++ccheck; break;	// ���������� �� ĭ �̵��� �˻�(���� ������ ��) 
		case 2: j = 0; ++i; ++ccheck; break;	// ������ ���� �����ߴٸ� �Ʒ� �ٷ� �Ѿ�� �˻�(������ ���� ������ ��)
		case 3: j = 0; ++i; break;	// ������ ���� �����ߴٸ� �Ʒ� �ٷ� �Ѿ�� �˻�(���� �ٲ� �� ó������ �˻�) 
		case 4:  ++j; break;	// ���������� �� ĭ �̵��� �˻�(���� ������ �ʾ��� ��) 
		}
	}

	i = 0; j = 0;

	while (1){	// row�� 3���̻� ������ ���ڰ� �ִ��� �˻��ؼ� 0���� �ٲ�
		if (j >= col)
		break;
		
		switch (RowCheck(RA, i, j, row, col)) {
			case 1 : ++i; ++rcheck; break; // �Ʒ��� �� ĭ �̵��� �˻�(���� ������ ��)
			case 2 : i = 0; ++j; ++rcheck; break;	// �Ʒ� ���� �����ߴٸ� ������ �ٷ� �Ѿ�� �˻�(������ ���� ������ ��)
			case 3 : i = 0; ++j; break;	// �Ʒ� ���� �����ߴٸ� ������ �ٷ� �Ѿ�� �˻�(���� �ٲ� �� ó������ �˻�) 
			case 4 : ++i; break;	// �Ʒ��� �� ĭ �̵��� �˻�(���� ������ �ʾ��� ��)
		}
	}

	for (i = 0; i < row; i++) {	// ������ �ٲ� �� �迭�� �̿��� ���� �迭 ���� ��� 
		for (j = 0; j < col; j++) {
			if (RA[i][j] == 0 || CA[i][j] == 0)	// �� �� �ϳ��� 0�̸� board�� 0 
			board[i][j] = 0;
			else board[i][j] = RA[i][j];	// �� �� ��� 0�� �ƴϸ� board�� �� �� �� �ϳ�. board[i][j] = CA[i][j] �Ǵ� board[i][j] = (RA[i][j] + CA[i][j]) / 2�� ���ų� ��ŵ�ص� ����
		}
	}

	if(rcheck || ccheck)	// �� ���� ��ȭ�� ���ٸ�( = 0) ���� ���Ḧ ���� 0 ��ȯ
	return 0;
	
	return 1;
}

void UpdateBoard(int board[MaxArr][MaxArr], int row, int col) {	// sorting �̿� 
	int i, j;

	for (i = 0; i < row; i++) {	// ZeroUp �Լ��� ���� ���� ������ �˻��ؼ� 0�� ���� �ø��� �۾��� �Ѵ�. �̸� ��� ���� ���� ����. 
		for (j = 0; j < col; j++)
		ZeroUp(board, i, j);
	}
}

void SubstBoard(int Arr[MaxArr][MaxArr], int Brr[MaxArr][MaxArr], int row, int col) {
	int a, b;
	
	for (a = 0; a < row; a++) {
		for (b = 0; b < col; b++) {
			Arr[a][b] = Brr[a][b];
		}
	}	
}

void ZeroUp(int board[MaxArr][MaxArr], int i, int j) {	// sorting �̿� 
	
	for (i; i > 0; --i) {
			if (board[i][j] == 0)	// �迭 ���� 0�̸� ���� 1ĭ �ø���. 
			swap(&board[i - 1][j], &board[i][j]);
	}
}

void EndBoard(int board[MaxArr][MaxArr], int row, int col) {
	FILE* result;
	int i, j;
	
	result = fopen("result.txt", "w");
	
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			fprintf(result, "%d ", board[i][j]);
		}
		fprintf(result, "\n");
	}
	
	fclose(result);
}

void FillBlank(int board[MaxArr][MaxArr], int row, int col) {
	int i, j;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (board[i][j] == 0)
				board[i][j] = rand() % 7 + 1;
		}
	}
}

void clear() {
	system("cls");
}

void enter() {
	printf("Press Enter to continue...\n");
	while (getchar() != '\n');
}

int ColCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col) {
	if (j >= col - 2)
		return 3;

	if (board[i][j] == board[i][j + 1] && board[i][j + 1] == board[i][j + 2]) {	// 3���� ���� ���ڰ� �����Ǿ� �ִ°�? 
		board[i][j] = 0;
		
		if (j < col - 3) {
			if (board[i][j + 2] != board[i][j + 3]) {	// �� ���� ���ڴ� �ٸ� �����ΰ�?
				board[i][j + 1] = 0;
				board[i][j + 2] = 0;
			}

			return 1;
		}
		
		else {
			board[i][j + 1] = 0;
			board[i][j + 2] = 0;
			return 2;
		}
	}


	return 4;
}

int RowCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col) {	// column�� row�� �ٲ� ColCheck �Լ��� ���� ������� ���� 
	if (i >= row - 2)
		return 3;

	if (board[i][j] == board[i + 1][j] && board[i + 1][j] == board[i + 2][j]) {
		board[i][j] = 0;
		
		if (i < row - 3) {
			if (board[i + 2][j] != board[i + 3][j]) {
				board[i + 1][j] = 0;
				board[i + 2][j] = 0;
			}

			return 1;
		}
		
		else {
			board[i + 1][j] = 0;
			board[i + 2][j] = 0;
			return 2;
		}
	}
	
	return 4;
}

void swap(int *p, int *q) {
	int temp;

	temp = *p;
	*p = *q;
	*q = temp;
}
