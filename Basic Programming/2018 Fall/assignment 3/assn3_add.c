#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MaxArr 12
#define GRY "\x1b[100m"	// ���󺯰� gray 
#define ORG "\x1b[101m"	// ���󺯰� orange 
#define GRN "\x1b[102m"	// ���󺯰� green
#define YEL "\x1b[103m"	// ���󺯰� yellow
#define BLU "\x1b[104m"	// ���󺯰� blue
#define PUP "\x1b[105m"	// ���󺯰� purple
#define SKY "\x1b[106m"	// ���󺯰� skyblue 
#define WHI "\x1b[107m"	// ���󺯰� white
#define RED "\x1b[41m"	// ���󺯰� red
#define BLK "\x1b[40m"	// ���󺯰� black
#define blk "\x1b[30m"	// ���󺯰�(����) black
#define RES "\x1b[0m"	// ���� reset

int show_menu();	// ���� �޴� ���� 
int start_game(int* S, int* T, int* R);	// ���� ���� �Լ� 
int ScoreMenu(int score, int turn, int R);	// �޴����� ���ھ� Ȯ�� ȭ���� �����ϴ� �Լ�
void Tutorial();	// ���ӿ� �ʿ��� board.txt ���Ͽ� ���� ����� ���� ���� ��� ����
int ReadBoard(int board[MaxArr][MaxArr], int* row, int* col);
void PrintBoard(int board[MaxArr][MaxArr], int row, int col);
int RemoveChunk(int board[MaxArr][MaxArr], int row, int col, int* score);
void UpdateBoard(int board[MaxArr][MaxArr], int row, int col);
void SubstBoard(int Arr[MaxArr][MaxArr], int Brr[MaxArr][MaxArr], int row, int col);	// Arr�� ���� Brr�� ������ substitution 
int ZeroUp(int board[MaxArr][MaxArr], int i, int j);	// board�� ǥ�õ� 0�� ���� �ø��� �Լ�
void EndBoard(int board[MaxArr][MaxArr], int row, int col, int score, int turn, int R);	// result.txt ����� �Լ�
void RandFill(int board[MaxArr][MaxArr], int row, int col);	// ������ ������ ������ �κп� ä��� �Լ� 
void clear();	// ȭ�� clear
void enter();	// Enter�� �Է¹޴� �Լ� 
int ColCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col, int* score);	// column�� 3���̻� ���� ���ڰ� �����Ǿ� �ִ��� Ȯ��
int RowCheck(int board[MaxArr][MaxArr], int CA[MaxArr][MaxArr], int i, int j, int row, int col, int* score);	// row�� 3���̻� ���� ���ڰ� �����Ǿ� �ִ��� Ȯ�� 
void swap(int *p, int *q);
void line(int a);	// ���ϴ� ������ŭ ��(*)�� ����ִ� �Լ� 
void end();	// ���� ���� �Լ� 
void error();	// �߸��� �� �Է½� ������ ����ִ� �Լ�.
void CalScore(int board[MaxArr][MaxArr], int i, int j, int* score);		// ���� �ǽð� ���
void PrintScore(int* score, int turn);	// ���� �ǽð� ǥ�� 
void ShuffleBoard(int board[MaxArr][MaxArr], int row, int col); //board�� �����ִ� �Լ� 
void Mission(int R, int* score);	// mission ���� ǥ�� �Լ� 
int MRange(int row, int col);		// board�� ���¿� ���� �ٸ� mission ���� ���� 

int main() {
	int menu, score = 0, turn = 0, S = 0, T = 0, R = 0;
	srand(time(NULL));

	while (1) {
		menu = show_menu();
		switch (menu) {
		case 1:
			start_game(&S, &T, &R);
			break;
		case 2:
			ScoreMenu(S, T, R);
			break;
		case 3:
			Tutorial();
			break;
		case 4:
			end();
		default:
			error();
		}
	}
	return 0;
}

int show_menu() {
	int select;

	clear();
	line(9);
	printf(" Simple Pang ");
	line(12);
	printf("\n\n");
	printf("	1. Game Start	\n\n");
	printf("	2. Score	\n\n");
	printf("	3. Tutorial	\n\n"); 
	printf("	4. Exit		\n\n");
	line(34);
	printf("\n\n");
	printf("Input : ");

	scanf("%d", &select);
	return select;
}

int start_game(int* S, int* T, int* R) {
	int row, col, i = 0, j = 0, score = 0, turn = 1, Rn;
	int board[MaxArr][MaxArr] = {0};

	switch(ReadBoard(board, &row, &col)) {
		case 100 :	// board.txt ���� x 
			printf("Error code : 100\n"); end();
		case 201 :	// range(1 ~ 5) ���� ���� ����
			printf("Error code : 201\n"); end();
		case 202 :	// range(1 ~ 7) ���� ���� ����
			printf("Error code : 202\n"); end();
		case 203 :	// range(1 ~ 9) ���� ���� ����
			printf("Error code : 203\n"); end();
		case 300 :	// �ʿ��� ���ڸ�ŭ ������� ���� ��
			printf("Error code : 300\n"); end();
		case 400 :	// row�� ������ ����� ��
			printf("Error code : 400\n"); end();
		case 500 :	// column�� ������ ����� ��
			printf("Error code : 500\n"); end(); 
		case 1 :
			break;
	}
	
	Rn = MRange(row, col);
	
	while (getchar() != '\n');	// Ű���� ���� �ʱ�ȭ
	clear();	// ����ȭ���� ���� �� ����ȭ�� ���� 
	Mission(Rn, &score);
	PrintScore(&score, turn);
	PrintBoard(board, row, col);
	printf("\n"); line(30); printf("\n\n");
	enter(); clear();
	
	
	do {
		if(RemoveChunk(board, row, col, &score)) {	// board�� ���̻� �ٲ��� ���� �� �����
			Mission(Rn, &score); 
			PrintScore(&score, turn);
			PrintBoard(board, row, col);
			printf("\n"); line(30); printf("\n\n");
			printf("No more patterns to remove.\n");
			if(score < Rn)
			printf("Mission failed.\n");
			printf("Press Enter to return to main.\n");
			while(getchar() != '\n');
			break;
		}
		
		Mission(Rn, &score);
		PrintScore(&score, turn);
		PrintBoard(board, row, col);	// ������ board ��� 
		printf("\n"); line(30); printf("\n\n");
		enter(); clear();
		
		Mission(Rn, &score);
		UpdateBoard(board, row, col);
		PrintScore(&score, turn);
		PrintBoard(board, row, col);	// �Ʒ��� ���ĵ� board ��� 
		printf("\n"); line(30); printf("\n\n");
		enter(); clear();
		
		Mission(Rn, &score);
		RandFill(board, row, col);
		PrintScore(&score, turn);
		PrintBoard(board, row, col);	// ������ ĭ�� �����ϰ� ä���� board ��� 
		printf("\n"); line(30); printf("\n\n");
		enter(); clear();
		
		Mission(Rn, &score);
		ShuffleBoard(board, row, col); ++turn;
		PrintScore(&score, turn);
		PrintBoard(board, row, col);	// cell�� �����ϰ� ���� board ��� 
		printf("\n"); line(30); printf("\n\n");
		enter(); clear();
	} while(1);
	
	EndBoard(board, row, col, score, turn, Rn);
	*S = score; *T = turn; *R = Rn;
	
	return 0;
}

int ScoreMenu(int score, int turn, int R) {
	
	clear();
	line(34);
	printf("\n\n");
	printf("Final Score : %d\n\n", score);
	printf("Final Turn : %d\n\n", turn);
	printf("Mission : %d\n\n", R);
	line(34);
	printf("\n\n");
	printf("Press Enter to go back to main.");
	while (getchar() != '\n');
	getchar();
	
	return 0;
}

void Tutorial() {
	clear();
	line(50);
	printf(" Simple Pang tutorial ");
	line(58); printf("\n\n");
	printf("1. Check your file 'board.txt'\n");
	printf("* The file'board.txt' should be configured as shown in the example below.\n\n");
	printf("5 5	<- row & col\n");
	printf("1 2 3 4 5\n");
	printf("3 4 3 ...	<- cell = row * col\n\n");
	printf("* The range of row and column is from 5 to 11, respectively.\n");
	printf("* Simple Pang supports different modes depending on the value of (row + col).\n");
	printf("** small : 10 to 13	medium : 14 to 17	large : 18 to 22\n");
	printf("* Each mode has a different range of number in cells.\n");
	printf("** small : 1 to 5	medium : 1 to 7		large : 1 to 9\n");
	printf("* If you start the game without writing 'board.txt' as written above, you will see an error code.\n\n");
	printf("2. Start & enjoy game\n");
	printf("* If three or more of the same numbers are listed in series, they will be cleared(horizontally & vertically) and scored.\n");
	printf("** The remaining numbers are sorted down.\n");
	printf("*** The empty cells aligned up and randomly fill the numbers that match the range of the mode.\n");
	printf("**** The cell of each digit are mixed and repeat from the beginning.\n");
	printf("* Try to reach your mission score!\n\n");
	line(130); printf("\n\n");
	
	printf("Press Enter to go back to main.");
	while (getchar() != '\n');
	getchar();
}

int ReadBoard(int board[MaxArr][MaxArr], int* row, int* col) {
	FILE* flboard;
	int i = 0, j = 0, check, RC;
	
	flboard = fopen("board.txt", "r");
	if(flboard == NULL) {
		printf("\aFile opening error : The file 'board.txt' does not exist.\n");
		return 100;
	}
	
	fscanf(flboard, "%d%d", row, col);
	if(*row < 5 || *row > 11) {	// row�� ������ �߸��� 
		printf("\aFile opening error : The file 'board.txt' has the wrong row range.(range : 5 to 11)\n");
		return 400;
	}
	if(*col < 5 || *col > 11) {	// col�� ������ �߸��� 
		printf("\aFile opening error : The file 'board.txt' has the wrong column range.(range : 5 to 11)\n");
		return 500;
	}
	
	RC = *row + *col;
	
	for(i = 0; i < *row; i++) {
		if(i > *row && board[i][j]) {
			}
		for(j = 0; j < *col; j++) {
			check = fscanf(flboard, "%d", &board[i][j]);
			if(check != 1) {	// check�� ���� �ִٸ� 1�� ��ȯ�ϹǷ� 1�� �ƴ϶�� �ʿ��� ���ں��� ���� ������ ���ڰ� �ԷµǾ� ���� 
				printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
				return 300;
			}
			if(RC <= 13)
				if((board[i][j] < 1) || (board[i][j] > 5)) {	// board row + col�� 10 ~ 13�̸� cell�� �� ������ ������ 1 ~ 5 
					printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : 1 to 5)\n");
					return 201;		
				}
			if(14 <= RC && RC <= 17)
				if((board[i][j] < 1) || (board[i][j] > 7)) {	// board row + col�� 14 ~ 17�̸� cell�� �� ������ ������ 1 ~ 7
					printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : 1 to 7)\n");
					return 202;		
				}
			if(RC >= 18)
				if((board[i][j] < 1) || (board[i][j] > 9)) {	// board row + col�� 10 ~ 13�̸� cell�� �� ������ ������ 1 ~ 9 
					printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : 1 to 9)\n");
					return 203;		
				}
		}
	}
	
	check = fscanf(flboard, "%d", &board[i][j]); // �������� �Է¹��� ��� ������ŭ �˻��ϹǷ� �Է� ���� �� ���� �ִ��� �� �� �� �˻� 
	if(check == 1) {	// check �� 1�� ��ȯ�Ѵٸ� �ʿ��� ���ں��� ���� ������ ���ڰ� �ԷµǾ� ���� 
		printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
		return 300;
	}
	
	fclose(flboard);
	
	return 1;	// �������� �����̸� 1�� ��ȯ
}

void PrintBoard(int board[MaxArr][MaxArr], int row, int col) {
	int i, j;
	
	for(i = 0; i < row; i++) {
		for(j = 0; j < col; j++) {
			if(board[i][j] == 0) printf(BLK);	// ���ڿ� �´� ������ ����
			if(board[i][j] == 1) printf(blk ORG);
			if(board[i][j] == 2) printf(blk GRN);
			if(board[i][j] == 3) printf(blk YEL);
			if(board[i][j] == 4) printf(blk BLU);
			if(board[i][j] == 5) printf(blk PUP);
			if(board[i][j] == 6) printf(blk SKY);
			if(board[i][j] == 7) printf(blk WHI);
			if(board[i][j] == 8) printf(blk RED);
			if(board[i][j] == 9) printf(blk GRY);

			printf("%d ", board[i][j]);
			printf(RES); 
		}
		printf("\n");
	}
	
	
}

int RemoveChunk(int board[MaxArr][MaxArr], int row, int col, int* score) {
	int RA[MaxArr][MaxArr] = {0}, CA[MaxArr][MaxArr] = {0};
	int i = 0, j = 0, ccheck = 0, rcheck = 0;	// ccheck, rcheck�� ���� column�� row�� ��ȭ�� �ִ��� check�ϴ� ���� 
	
	SubstBoard(RA, board, row,col);	// board�� RA�� ���� 
	SubstBoard(CA, board, row, col);	// board�� CA�� ���� 
	
	while(1) {	// column�� 3���̻� ������ ���ڰ� �ִ��� �˻��ؼ� 0���� �ٲ�
		if(i > row)
		break;
		
		switch(ColCheck(CA, i, j, row, col, &*score)) {
			case 1 : ++j; ++ccheck; break;	// ���������� �� ĭ �̵��� �˻�(���� ������ ��) 
			case 2 : 
				if(j > col) {
				++i; ++ccheck; break;	// ������ ���� �����ߴٸ� �Ʒ� �ٷ� �Ѿ�� �˻�(������ ���� ������ ��) 
				}
				else
				++j; ++ccheck; break;	// ������ ���� �������� �ʾҴٸ� ���������� �� ĭ �̵��� �˻� 
			case 3 : j = 0; ++i; break;	// ������ ���� �����ߴٸ� �Ʒ� �ٷ� �Ѿ�� �˻�(�� ����� �Ͱ� ������� ������ ���� �ٲ� �� ó������ �˻�) 
			case 4 :  ++j; break;	// ���������� �� ĭ �̵��� �˻�(���� ������ �ʾ��� ��) 
		}
	}
	
	i = 0; j = 0;
	
	while(1) {	// row�� 3���̻� ������ ���ڰ� �ִ��� �˻��ؼ� 0���� �ٲ�
		if(j > col)
		break;
		
		switch(RowCheck(RA, CA, i, j, row, col, &*score)) {
			case 1 : ++i; ++rcheck; break; // �Ʒ��� �� ĭ �̵��� �˻�(���� ������ ��)
			case 2 : 
				if(i > row) {
				++j; ++rcheck; break;	// �Ʒ� ���� �����ߴٸ� ������ �ٷ� �Ѿ�� �˻�(������ ���� ������ ��) 
				}
				else
				++i; ++rcheck; break;	// �Ʒ� ���� �������� �ʾҴٸ� �Ʒ��� �� ĭ �̵��� �˻� 
			case 3 : i = 0; ++j; break;	// �Ʒ� ���� �����ߴٸ� ������ �ٷ� �Ѿ�� �˻�(�� ����� �Ͱ� ������� ������ ���� �ٲ� �� ó������ �˻�) 
			case 4 : ++i; break;	// �Ʒ��� �� ĭ �̵��� �˻�(���� ������ �ʾ��� ��)
		}
	}
	
	for(i = 0; i < row; i++) {	// ������ �ٲ� �� �迭�� �̿��� ���� �迭 ���� ��� 
		for(j = 0; j < col; j++) {
			if(RA[i][j] == 0 || CA[i][j] == 0)	// �� �� �ϳ��� 0�̸� board�� 0 
			board[i][j] = 0;
			else board[i][j] = RA[i][j];	// �� �� ��� 0�� �ƴϸ� board�� �� �� �� �ϳ�. board[i][j] = CA[i][j] �Ǵ� board[i][j] = (RA[i][j] + CA[i][j]) / 2�� �ᵵ ���� 
		}
	}
	
	if(rcheck || ccheck)	// �� ���� ��ȭ�� ���ٸ�( = 0) ���� ���Ḧ ���� 0 ��ȯ
	return 0;
	
	return 1;
}

void UpdateBoard(int board[MaxArr][MaxArr], int row, int col) {	// sorting �̿� 
	int i, j;

	for(i = 0; i < row; i++) {	// ZeroUp �Լ��� ���� ���� ������ �˻��ؼ� 0�� ���� �ø��� �۾��� �Ѵ�. �̸� ��� ���� ���� ����. 
		for(j = 0; j < col; j++)
		ZeroUp(board, i, j);
	}
}

void SubstBoard(int Arr[MaxArr][MaxArr], int Brr[MaxArr][MaxArr], int row, int col) {
	int a, b;
	
	for(a = 0; a < row; a++) {
		for(b = 0; b < col; b++) {
			Arr[a][b] = Brr[a][b];
		}
	}	
}

int ZeroUp(int board[MaxArr][MaxArr], int i, int j) {	// sorting �̿� 
	
	for(i; i > 0; --i) {
			if(board[i][j] == 0)	// �迭 ���� 0�̸� ���� 1ĭ �ø���. 
			swap(&board[i - 1][j], &board[i][j]);
	}
}

void EndBoard(int board[MaxArr][MaxArr], int row, int col, int score, int turn, int R) {
	FILE* result;
	int i, j;
	
	result = fopen("result.txt", "w");
	
	for(i = 0; i < row; i++) {
		for(j = 0; j < col; j++) {
			fprintf(result, "%d ", board[i][j]);
		}
		fprintf(result, "\n");
	}
	fprintf(result, "Final Score : %d\n", score);
	fprintf(result, "Final Turn : %d\n", turn);
	fprintf(result, "Mission : %d\n", R);
	
	fclose(result);
}

void RandFill(int board[MaxArr][MaxArr], int row, int col) {
	int fill, i, j, RC;
	
	RC = row + col;
	for(i = 0; i < row; i++) {
		for(j = 0; j < col; j++) {	// for���� �������� ���� �������� ������ �迭 ���� 0�� ���� ����
			if(RC <= 13)	// row + col�� ���� ���� ���� �ٸ� ������ ���ڸ� ä�� 
			fill = (rand() % 5) + 1;
			if(14 <= RC && RC <= 17)
			fill = (rand() % 7) + 1;
			if(RC >= 18)
			fill = (rand() % 9) + 1;
			
			if(board[i][j] == 0)
			board[i][j] = fill;
		}
	}
}

void clear() {
	system("clear");
}

void enter() {
	printf("Press Enter to continue...\n");
	while(getchar() != '\n'); 
}

int ColCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col, int* score) {


	if(board[i][j] > 0 && board[i][j] == board[i][j + 1] && board[i][j + 1] == board[i][j + 2]) {	// 3���� ���� ���ڰ� �����Ǿ� �ִ°�? 
		CalScore(board, i, j, score);	// ����� �� ���� ��� 
		board[i][j] = 0;
		
		if(board[i][j + 2] > 0 && board[i][j + 2] == board[i][j + 3])	// �� ���� ���ڵ� ���� �����ΰ�? 
		return 1;
		
		else {
			CalScore(board, i, j + 1, score);
			CalScore(board, i, j + 2, score);
			board[i][j + 1] = 0;
			board[i][j + 2] = 0;
			return 2;
		}
	}
	
	if(j >= col)
	return 3;

	return 4;
}

int RowCheck(int board[MaxArr][MaxArr],	int CA[MaxArr][MaxArr], int i, int j, int row, int col, int* score) {	// column�� row�� �ٲ� ColCheck �Լ��� ���� ������� ���� 
	
	if(board[i][j] > 0 && board[i][j] == board[i + 1][j] && board[i + 1][j] == board[i + 2][j]) {
		if(CA[i][j] != 0)	// �� ������ �κ��� ������ ����� ���� �ߺ� ����� ���� ���� 
		CalScore(board, i, j, score);
		board[i][j] = 0;
		
		if(board[i + 2][j] > 0 && board[i + 2][j] == board[i + 3][j])
		return 1;
		
		else {
			if(CA[i + 1][j] != 0)
				CalScore(board, i + 1, j, score);
			if(CA[i + 2][j] != 0)
				CalScore(board, i + 2, j, score);
			board[i + 1][j] = 0;
			board[i + 2][j] = 0;
			return 2;
		}
	}
	
	if(i >= row)
	return 3;
	
	return 4;
}

void swap(int *p, int *q) {
	int temp;

	temp = *p;
	*p = *q;
	*q = temp;
}

void line(int a) {
	int i;
	for (i = 0; i < a; i++) {
		printf("*"); //���ϴ� ����(a)��ŭ �� ǥ�� 
	}
}

void end() {
	printf("\nExit the program....\n");
	exit(0); // ���α׷� ����
}

void error() {
	printf("\aPlease select the correct menu.\nPress Enter to go back to main.");
	while (getchar() != '\n');
	getchar();	// Enter ���� ���� ���� �Է��� ��� Ű���� ���۸� �ʱ�ȭ ���� �����߻��� ���� ����
	clear();
}

void CalScore(int board[MaxArr][MaxArr], int i, int j, int* score) {
	*score += board[i][j];
}

void PrintScore(int* score, int turn) {
	line(30); printf("\n\n");
	printf("Score : %d\n\n", *score);
	printf("Turn : %d\n\n", turn);
	line(30); printf("\n\n");
}

void ShuffleBoard(int board[MaxArr][MaxArr], int row, int col) {
	int i, R1, R2, R3, R4, temp;
	
	for(i = 0; i < 100; i++) {	// 100�� ���´� 
	R1 = rand() % row;
	R2 = rand() % col;
	R3 = rand() % row;
	R4 = rand() % col;
	
	temp = board[R1][R2];	// swap �Լ� ���� 
	board[R1][R2] = board[R3][R4];
	board[R3][R4] = temp;
	}
}

void Mission(int R, int* score) {
	line(30); printf("\n\n");
	printf("Your mission : get %d point\n\n", R);
	if(*score >= R)
	printf("Mission Clear!\n\n", R);
	line(30); printf("\n\n");
}

int MRange(int row, int col) { 
	int range, max = 100, min = 50, i, RC, Rn;
	
	RC = row + col - 10;	// row + col�� ������ 10 ~ 22
	for(i = 0; i < RC; i++) {	// board�� ũ�⿡ ���� mission score�� �ִ� �ּ� ���� ��ȭ 
		max += 100; 
		min += 50;
	}	// ex). (5,6) or (6,5) board�� ������ mission score�� ������ 100 ~ 200, (11, 11) board�� ������ mission score�� ������  650 ~ 1300
	range = (max - min + 1); 
	Rn = rand() % range + min;
	
	return Rn;
}
