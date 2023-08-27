#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MaxArr 10
#define GRY "\x1b[90m"	// 색상변경 gray 
#define ORG "\x1b[91m"	// 색상변경 orange 
#define GRN "\x1b[92m"	// 색상변경 green
#define YEL "\x1b[93m"	// 색상변경 yellow
#define BLU "\x1b[94m"	// 색상변경 blue
#define PUP "\x1b[95m"	// 색상변경 purple
#define SKY "\x1b[96m"	// 색상변경 skyblue 
#define WHI "\x1b[97m"	// 색상변경 white 
#define RES "\x1b[0m"	// 색상 reset

int ReadBoard(int board[MaxArr][MaxArr], int* row, int* col);
void PrintBoard(int board[MaxArr][MaxArr], int row, int col);
int RemoveChunk(int board[MaxArr][MaxArr], int row, int col);
void UpdateBoard(int board[MaxArr][MaxArr], int row, int col);
void SubstBoard(int Arr[MaxArr][MaxArr], int Brr[MaxArr][MaxArr], int row, int col);	// Arr의 값을 Brr의 값으로 substitution 
void ZeroUp(int board[MaxArr][MaxArr], int i, int j);	// board에 표시된 0을 위로 올리는 함수
void EndBoard(int board[MaxArr][MaxArr], int row, int col);	// result.txt 만드는 함수
void FillBlank(int board[MaxArr][MaxArr], int row, int col);
void clear();	// 화면 clear
void enter();	// Enter를 입력받는 함수 
int ColCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col);	// column에 3개이상 같은 숫자가 나열되어 있는지 확인
int RowCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col);	// row에 3개이상 같은 숫자가 나열되어 있는지 확인 
void swap(int *p, int *q);

int main() {
	int row, col, i = 0, j = 0;
	int board[MaxArr][MaxArr] = {0};
	
	srand(time(NULL));

	switch (ReadBoard(board, &row, &col)) {
		case 100 :	// board.txt 존재 x 
			printf("Error code : 100\n");
			return 0;
		case 200 :	// range(1 ~ 7) 밖의 숫자 존재
			printf("Error code : 200\n");
			return 0;
		case 300 :	// 필요한 숫자만큼 들어있지 않을 때
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
	
		if (RemoveChunk(board, row, col)) {	// board가 더이상 바뀌지 않을 때 실행됨 
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
			if (check != 1) {	// check에 값이 있다면 1을 반환하므로 1이 아니라면 필요한 숫자보다 작은 개수의 숫자가 입력되어 있음 
				printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
				return 300;
			}
			if ((board[i][j] < 1) || (board[i][j] > 7)) {
				printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : 1 to 7)\n");
				return 200;		
			}
		}
	}
	
	check = fscanf(flboard, "%d", &board[i][j]); // 위에서는 입력받은 행렬 갯수만큼 검사하므로 입력 값이 더 남아 있는지 한 번 더 검사 
	if (check == 1) {	// check 가 1을 반환한다면 필요한 숫자보다 많은 개수의 숫자가 입력되어 있음 
		printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
		return 300;
	}
	
	fclose(flboard);
	
	return 1;	// 오류없이 정상이면 1을 반환
}

void PrintBoard(int board[MaxArr][MaxArr], int row, int col) {
	int i, j;
	
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			switch (board[i][j]) {
				case 0:
					printf(GRY); break;		// 숫자에 맞는 색으로 변경
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
	int i = 0, j = 0, ccheck = 0, rcheck = 0;	// ccheck, rcheck는 각각 column과 row에 변화가 있는지 check하는 변수 

	SubstBoard(RA, board, row, col);	// board를 RA에 저장 
	SubstBoard(CA, board, row, col);	// board를 CA에 저장 

	while (1) {	// column에 3개이상 나열된 숫자가 있는지 검사해서 0으로 바꿈
		if (i >= row)
			break;
		switch (ColCheck(CA, i, j, row, col)) {
		case 1: ++j; ++ccheck; break;	// 오른쪽으로 한 칸 이동해 검사(값을 지웠을 때) 
		case 2: j = 0; ++i; ++ccheck; break;	// 오른쪽 끝에 도착했다면 아래 줄로 넘어가서 검사(끝에서 값을 지웠을 때)
		case 3: j = 0; ++i; break;	// 오른쪽 끝에 도착했다면 아래 줄로 넘어가서 검사(줄을 바꾼 후 처음부터 검사) 
		case 4:  ++j; break;	// 오른쪽으로 한 칸 이동해 검사(값을 지우지 않았을 때) 
		}
	}

	i = 0; j = 0;

	while (1){	// row에 3개이상 나열된 숫자가 있는지 검사해서 0으로 바꿈
		if (j >= col)
		break;
		
		switch (RowCheck(RA, i, j, row, col)) {
			case 1 : ++i; ++rcheck; break; // 아래로 한 칸 이동해 검사(값을 지웠을 때)
			case 2 : i = 0; ++j; ++rcheck; break;	// 아래 끝에 도착했다면 오른쪽 줄로 넘어가서 검사(끝에서 값을 지웠을 때)
			case 3 : i = 0; ++j; break;	// 아래 끝에 도착했다면 오른쪽 줄로 넘어가서 검사(줄을 바꾼 후 처음부터 검사) 
			case 4 : ++i; break;	// 아래로 한 칸 이동해 검사(값을 지우지 않았을 때)
		}
	}

	for (i = 0; i < row; i++) {	// 위에서 바꾼 두 배열을 이용해 최종 배열 형태 계산 
		for (j = 0; j < col; j++) {
			if (RA[i][j] == 0 || CA[i][j] == 0)	// 둘 중 하나가 0이면 board는 0 
			board[i][j] = 0;
			else board[i][j] = RA[i][j];	// 두 값 모두 0이 아니면 board는 두 값 중 하나. board[i][j] = CA[i][j] 또는 board[i][j] = (RA[i][j] + CA[i][j]) / 2로 쓰거나 스킵해도 무관
		}
	}

	if(rcheck || ccheck)	// 두 값에 변화가 없다면( = 0) 게임 종료를 위해 0 반환
	return 0;
	
	return 1;
}

void UpdateBoard(int board[MaxArr][MaxArr], int row, int col) {	// sorting 이용 
	int i, j;

	for (i = 0; i < row; i++) {	// ZeroUp 함수로 가장 왼쪽 열부터 검사해서 0을 위로 올리는 작업을 한다. 이를 모든 열에 대해 시행. 
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

void ZeroUp(int board[MaxArr][MaxArr], int i, int j) {	// sorting 이용 
	
	for (i; i > 0; --i) {
			if (board[i][j] == 0)	// 배열 값이 0이면 위로 1칸 올린다. 
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

	if (board[i][j] == board[i][j + 1] && board[i][j + 1] == board[i][j + 2]) {	// 3개의 같은 숫자가 나열되어 있는가? 
		board[i][j] = 0;
		
		if (j < col - 3) {
			if (board[i][j + 2] != board[i][j + 3]) {	// 그 옆의 숫자는 다른 숫자인가?
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

int RowCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col) {	// column을 row로 바꿔 ColCheck 함수와 같은 방식으로 구성 
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
