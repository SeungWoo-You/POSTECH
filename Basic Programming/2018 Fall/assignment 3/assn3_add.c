#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MaxArr 12
#define GRY "\x1b[100m"	// 색상변경 gray 
#define ORG "\x1b[101m"	// 색상변경 orange 
#define GRN "\x1b[102m"	// 색상변경 green
#define YEL "\x1b[103m"	// 색상변경 yellow
#define BLU "\x1b[104m"	// 색상변경 blue
#define PUP "\x1b[105m"	// 색상변경 purple
#define SKY "\x1b[106m"	// 색상변경 skyblue 
#define WHI "\x1b[107m"	// 색상변경 white
#define RED "\x1b[41m"	// 색상변경 red
#define BLK "\x1b[40m"	// 색상변경 black
#define blk "\x1b[30m"	// 색상변경(글자) black
#define RES "\x1b[0m"	// 색상 reset

int show_menu();	// 메인 메뉴 실행 
int start_game(int* S, int* T, int* R);	// 게임 진행 함수 
int ScoreMenu(int score, int turn, int R);	// 메뉴에서 스코어 확인 화면을 구성하는 함수
void Tutorial();	// 게임에 필요한 board.txt 파일에 대한 설명과 게임 진행 방식 설명
int ReadBoard(int board[MaxArr][MaxArr], int* row, int* col);
void PrintBoard(int board[MaxArr][MaxArr], int row, int col);
int RemoveChunk(int board[MaxArr][MaxArr], int row, int col, int* score);
void UpdateBoard(int board[MaxArr][MaxArr], int row, int col);
void SubstBoard(int Arr[MaxArr][MaxArr], int Brr[MaxArr][MaxArr], int row, int col);	// Arr의 값을 Brr의 값으로 substitution 
int ZeroUp(int board[MaxArr][MaxArr], int i, int j);	// board에 표시된 0을 위로 올리는 함수
void EndBoard(int board[MaxArr][MaxArr], int row, int col, int score, int turn, int R);	// result.txt 만드는 함수
void RandFill(int board[MaxArr][MaxArr], int row, int col);	// 난수를 생성해 지워진 부분에 채우는 함수 
void clear();	// 화면 clear
void enter();	// Enter를 입력받는 함수 
int ColCheck(int board[MaxArr][MaxArr], int i, int j, int row, int col, int* score);	// column에 3개이상 같은 숫자가 나열되어 있는지 확인
int RowCheck(int board[MaxArr][MaxArr], int CA[MaxArr][MaxArr], int i, int j, int row, int col, int* score);	// row에 3개이상 같은 숫자가 나열되어 있는지 확인 
void swap(int *p, int *q);
void line(int a);	// 원하는 개수만큼 별(*)을 찍어주는 함수 
void end();	// 게임 종료 함수 
void error();	// 잘못된 값 입력시 오류를 띄워주는 함수.
void CalScore(int board[MaxArr][MaxArr], int i, int j, int* score);		// 점수 실시간 계산
void PrintScore(int* score, int turn);	// 점수 실시간 표시 
void ShuffleBoard(int board[MaxArr][MaxArr], int row, int col); //board를 섞어주는 함수 
void Mission(int R, int* score);	// mission 점수 표시 함수 
int MRange(int row, int col);		// board의 형태에 따라 다른 mission 점수 생성 

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
		case 100 :	// board.txt 존재 x 
			printf("Error code : 100\n"); end();
		case 201 :	// range(1 ~ 5) 밖의 숫자 존재
			printf("Error code : 201\n"); end();
		case 202 :	// range(1 ~ 7) 밖의 숫자 존재
			printf("Error code : 202\n"); end();
		case 203 :	// range(1 ~ 9) 밖의 숫자 존재
			printf("Error code : 203\n"); end();
		case 300 :	// 필요한 숫자만큼 들어있지 않을 때
			printf("Error code : 300\n"); end();
		case 400 :	// row의 범위가 벗어났을 때
			printf("Error code : 400\n"); end();
		case 500 :	// column의 범위가 벗어났을 때
			printf("Error code : 500\n"); end(); 
		case 1 :
			break;
	}
	
	Rn = MRange(row, col);
	
	while (getchar() != '\n');	// 키보드 버퍼 초기화
	clear();	// 메인화면을 지운 후 게임화면 실행 
	Mission(Rn, &score);
	PrintScore(&score, turn);
	PrintBoard(board, row, col);
	printf("\n"); line(30); printf("\n\n");
	enter(); clear();
	
	
	do {
		if(RemoveChunk(board, row, col, &score)) {	// board가 더이상 바뀌지 않을 때 실행됨
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
		PrintBoard(board, row, col);	// 지워진 board 출력 
		printf("\n"); line(30); printf("\n\n");
		enter(); clear();
		
		Mission(Rn, &score);
		UpdateBoard(board, row, col);
		PrintScore(&score, turn);
		PrintBoard(board, row, col);	// 아래로 정렬된 board 출력 
		printf("\n"); line(30); printf("\n\n");
		enter(); clear();
		
		Mission(Rn, &score);
		RandFill(board, row, col);
		PrintScore(&score, turn);
		PrintBoard(board, row, col);	// 지워진 칸이 랜덤하게 채워진 board 출력 
		printf("\n"); line(30); printf("\n\n");
		enter(); clear();
		
		Mission(Rn, &score);
		ShuffleBoard(board, row, col); ++turn;
		PrintScore(&score, turn);
		PrintBoard(board, row, col);	// cell을 랜덤하게 섞은 board 출력 
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
	if(*row < 5 || *row > 11) {	// row의 범위가 잘못됨 
		printf("\aFile opening error : The file 'board.txt' has the wrong row range.(range : 5 to 11)\n");
		return 400;
	}
	if(*col < 5 || *col > 11) {	// col의 범위가 잘못됨 
		printf("\aFile opening error : The file 'board.txt' has the wrong column range.(range : 5 to 11)\n");
		return 500;
	}
	
	RC = *row + *col;
	
	for(i = 0; i < *row; i++) {
		if(i > *row && board[i][j]) {
			}
		for(j = 0; j < *col; j++) {
			check = fscanf(flboard, "%d", &board[i][j]);
			if(check != 1) {	// check에 값이 있다면 1을 반환하므로 1이 아니라면 필요한 숫자보다 작은 개수의 숫자가 입력되어 있음 
				printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
				return 300;
			}
			if(RC <= 13)
				if((board[i][j] < 1) || (board[i][j] > 5)) {	// board row + col이 10 ~ 13이면 cell에 들어갈 숫자의 범위는 1 ~ 5 
					printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : 1 to 5)\n");
					return 201;		
				}
			if(14 <= RC && RC <= 17)
				if((board[i][j] < 1) || (board[i][j] > 7)) {	// board row + col이 14 ~ 17이면 cell에 들어갈 숫자의 범위는 1 ~ 7
					printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : 1 to 7)\n");
					return 202;		
				}
			if(RC >= 18)
				if((board[i][j] < 1) || (board[i][j] > 9)) {	// board row + col이 10 ~ 13이면 cell에 들어갈 숫자의 범위는 1 ~ 9 
					printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : 1 to 9)\n");
					return 203;		
				}
		}
	}
	
	check = fscanf(flboard, "%d", &board[i][j]); // 위에서는 입력받은 행렬 갯수만큼 검사하므로 입력 값이 더 남아 있는지 한 번 더 검사 
	if(check == 1) {	// check 가 1을 반환한다면 필요한 숫자보다 많은 개수의 숫자가 입력되어 있음 
		printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
		return 300;
	}
	
	fclose(flboard);
	
	return 1;	// 오류없이 정상이면 1을 반환
}

void PrintBoard(int board[MaxArr][MaxArr], int row, int col) {
	int i, j;
	
	for(i = 0; i < row; i++) {
		for(j = 0; j < col; j++) {
			if(board[i][j] == 0) printf(BLK);	// 숫자에 맞는 색으로 변경
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
	int i = 0, j = 0, ccheck = 0, rcheck = 0;	// ccheck, rcheck는 각각 column과 row에 변화가 있는지 check하는 변수 
	
	SubstBoard(RA, board, row,col);	// board를 RA에 저장 
	SubstBoard(CA, board, row, col);	// board를 CA에 저장 
	
	while(1) {	// column에 3개이상 나열된 숫자가 있는지 검사해서 0으로 바꿈
		if(i > row)
		break;
		
		switch(ColCheck(CA, i, j, row, col, &*score)) {
			case 1 : ++j; ++ccheck; break;	// 오른쪽으로 한 칸 이동해 검사(값을 지웠을 때) 
			case 2 : 
				if(j > col) {
				++i; ++ccheck; break;	// 오른쪽 끝에 도착했다면 아래 줄로 넘어가서 검사(끝에서 값을 지웠을 때) 
				}
				else
				++j; ++ccheck; break;	// 오른쪽 끝에 도착하지 않았다면 오른쪽으로 한 칸 이동해 검사 
			case 3 : j = 0; ++i; break;	// 오른쪽 끝에 도착했다면 아래 줄로 넘어가서 검사(값 지우는 것과 상관없이 무조건 줄을 바꾼 후 처음부터 검사) 
			case 4 :  ++j; break;	// 오른쪽으로 한 칸 이동해 검사(값을 지우지 않았을 때) 
		}
	}
	
	i = 0; j = 0;
	
	while(1) {	// row에 3개이상 나열된 숫자가 있는지 검사해서 0으로 바꿈
		if(j > col)
		break;
		
		switch(RowCheck(RA, CA, i, j, row, col, &*score)) {
			case 1 : ++i; ++rcheck; break; // 아래로 한 칸 이동해 검사(값을 지웠을 때)
			case 2 : 
				if(i > row) {
				++j; ++rcheck; break;	// 아래 끝에 도착했다면 오른쪽 줄로 넘어가서 검사(끝에서 값을 지웠을 때) 
				}
				else
				++i; ++rcheck; break;	// 아래 끝에 도착하지 않았다면 아래로 한 칸 이동해 검사 
			case 3 : i = 0; ++j; break;	// 아래 끝에 도착했다면 오른쪽 줄로 넘어가서 검사(값 지우는 것과 상관없이 무조건 줄을 바꾼 후 처음부터 검사) 
			case 4 : ++i; break;	// 아래로 한 칸 이동해 검사(값을 지우지 않았을 때)
		}
	}
	
	for(i = 0; i < row; i++) {	// 위에서 바꾼 두 배열을 이용해 최종 배열 형태 계산 
		for(j = 0; j < col; j++) {
			if(RA[i][j] == 0 || CA[i][j] == 0)	// 둘 중 하나가 0이면 board는 0 
			board[i][j] = 0;
			else board[i][j] = RA[i][j];	// 두 값 모두 0이 아니면 board는 두 값 중 하나. board[i][j] = CA[i][j] 또는 board[i][j] = (RA[i][j] + CA[i][j]) / 2로 써도 무관 
		}
	}
	
	if(rcheck || ccheck)	// 두 값에 변화가 없다면( = 0) 게임 종료를 위해 0 반환
	return 0;
	
	return 1;
}

void UpdateBoard(int board[MaxArr][MaxArr], int row, int col) {	// sorting 이용 
	int i, j;

	for(i = 0; i < row; i++) {	// ZeroUp 함수로 가장 왼쪽 열부터 검사해서 0을 위로 올리는 작업을 한다. 이를 모든 열에 대해 시행. 
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

int ZeroUp(int board[MaxArr][MaxArr], int i, int j) {	// sorting 이용 
	
	for(i; i > 0; --i) {
			if(board[i][j] == 0)	// 배열 값이 0이면 위로 1칸 올린다. 
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
		for(j = 0; j < col; j++) {	// for문을 돌때마다 값을 랜덤으로 생성해 배열 값이 0인 곳에 저장
			if(RC <= 13)	// row + col의 값에 따라 서로 다른 범위의 숫자를 채움 
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


	if(board[i][j] > 0 && board[i][j] == board[i][j + 1] && board[i][j + 1] == board[i][j + 2]) {	// 3개의 같은 숫자가 나열되어 있는가? 
		CalScore(board, i, j, score);	// 지우기 전 점수 계산 
		board[i][j] = 0;
		
		if(board[i][j + 2] > 0 && board[i][j + 2] == board[i][j + 3])	// 그 옆의 숫자도 같은 숫자인가? 
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

int RowCheck(int board[MaxArr][MaxArr],	int CA[MaxArr][MaxArr], int i, int j, int row, int col, int* score) {	// column을 row로 바꿔 ColCheck 함수와 같은 방식으로 구성 
	
	if(board[i][j] > 0 && board[i][j] == board[i + 1][j] && board[i + 1][j] == board[i + 2][j]) {
		if(CA[i][j] != 0)	// 안 지워진 부분의 점수만 계산해 점수 중복 계산을 막기 위함 
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
		printf("*"); //원하는 개수(a)만큼 별 표시 
	}
}

void end() {
	printf("\nExit the program....\n");
	exit(0); // 프로그램 종료
}

void error() {
	printf("\aPlease select the correct menu.\nPress Enter to go back to main.");
	while (getchar() != '\n');
	getchar();	// Enter 전에 여러 값을 입력한 경우 키보드 버퍼를 초기화 시켜 오류발생을 막기 위함
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
	
	for(i = 0; i < 100; i++) {	// 100번 섞는다 
	R1 = rand() % row;
	R2 = rand() % col;
	R3 = rand() % row;
	R4 = rand() % col;
	
	temp = board[R1][R2];	// swap 함수 응용 
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
	
	RC = row + col - 10;	// row + col의 범위는 10 ~ 22
	for(i = 0; i < RC; i++) {	// board의 크기에 맞춰 mission score의 최대 최소 범위 변화 
		max += 100; 
		min += 50;
	}	// ex). (5,6) or (6,5) board는 가능한 mission score의 범위가 100 ~ 200, (11, 11) board는 가능한 mission score의 범위가  650 ~ 1300
	range = (max - min + 1); 
	Rn = rand() % range + min;
	
	return Rn;
}
