#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define MAX_SIZE 10  // 최대 배열 크기
#define MIN_TILE 1  // 최소 타일 번호
#define MAX_TILE 8  // 최대 타일 번호
#define LINE 24  // 한 줄에 들어가는 문자 수

enum ColorType {  // 열거형을 이용한 색깔놀이~(쓸데 없음)
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

int make_board(int board[MAX_SIZE][MAX_SIZE], int* row, int* col);  // board.txt 파일 읽고 보드판 데이터 저장
void print_board(int board[MAX_SIZE][MAX_SIZE], int row, int col);  // 보드판 출력
int remove_tiles(int board[MAX_SIZE][MAX_SIZE], int row, int col);  // 타일 제거
void update_board(int board[MAX_SIZE][MAX_SIZE], int row, int col);  // 타일 업데이트
void Copy_Board(int from_board[MAX_SIZE][MAX_SIZE], int to_board[MAX_SIZE][MAX_SIZE], int row, int col); // 보드판 복사
int Check_Square(int board[MAX_SIZE][MAX_SIZE], int row, int col);  // 타일 지울 수 있는지(2X2 정사각형인지) 확인
int Delete_Tile(int board[MAX_SIZE][MAX_SIZE], int row, int col); // 타일 지우기(0으로 바꾸고 지워진 횟수 카운트)
void Print_Result(int change_count);  // 최종 결과 출력(콘솔)
void Print_Result(int change_count, FILE* file); // 최종 결과 출력(파일)
void End_Board(int board[MAX_SIZE][MAX_SIZE], int row, int col, int change_count);  // result.txt 파일 쓰기
void Text_Color(int colorNum);  // 타일 숫자 색 변경(쓸데 없음)
void line(int num);  // 줄 긋기(콘솔)
void line(int num, FILE* file);  // 줄 긋기(파일)
void enter();  // 엔터 입력 받기
void clear();  // 화면 지우기
void swap(int* p, int* q);  // 두 타일 위치 교환

int main() {
	int row = MAX_SIZE, col = MAX_SIZE, i = 0, j = 0, change_count = 0, temp_count = 0;
	int board[MAX_SIZE][MAX_SIZE] = { 0 };

	switch (make_board(board, &row, &col)) {  // 오류 검사(외에도 보드 사이즈가 MAX_SIZE보다 크다거나 하는 등의 다양한 예외 처리 확장 가능)
	case 100:  // board.txt 존재 X
		printf("Error code : 100\n");
		return 0;
	case 200:  // range(1 ~ 8) 밖의 숫자 존재
		printf("Error code : 200\n");
		return 0;
	case 300:  // 필요한 숫자만큼 들어있지 않을 때
		printf("Error code : 300\n");
		return 0;
	case 1:
		break;
	}

	while (1) {  // 게임 진행
		print_board(board, row, col);
		temp_count = remove_tiles(board, row, col);

		if (temp_count == 0) {	// board가 바뀌지 않았을 때 -> 게임 종료 
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
	int i = 0, j = 0, check; // check: 오류 확인 용

	file_board = fopen("board.txt", "r");
	if (file_board == NULL) {  // 파일 없을 때
		printf("\aFile opening error : The file 'board.txt' does not exist.\n"); // \a: 알림음(경고음) 효과
		return 100;
	}

	fscanf(file_board, "%d%d", row, col);

	for (i = 0; i < *row; i++) {  // board data 입력
		for (j = 0; j < *col; j++) {
			check = fscanf(file_board, "%d", &board[i][j]);
			if (check != 1) {	// check에 값이 있다면(스캔을 정상적으로 했다면) 1 반환 -> 1이 아니라면 필요한 숫자보다 작은 개수의 숫자가 입력되어 있음(또는 쓰레기 데이터)
				printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
				return 300;
			}

			if ((board[i][j] < MIN_TILE) || (board[i][j] > MAX_TILE)) {  // 보드에 범위 밖의 데이터 존재
				printf("\aFile opening error : The file 'board.txt' includes numbers out of range.(range : %d to %d)\n", MIN_TILE, MAX_TILE);
				return 200;
			}
		}
	}

	check = fscanf(file_board, "%d", &board[i][j]);  // 입력 값이 더 남아 있는지 한 번 더 검사 
	if (check == 1) {	// check가 1이면(스캔을 정상적으로 했다면) 필요한 숫자보다 많은 개수의 숫자가 입력되어 있음 
		printf("\aFile opening error : The file 'board.txt' does not match the number of cells.\n");
		return 300;
	}

	fclose(file_board);

	return 1;	// 오류없이 완료하면 1을 반환
}

void print_board(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	int i, j;

	clear();

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			switch (board[i][j]) {  // 숫자에 맞는 색으로 변경
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
		}   // 색깔놀이. switch case 부분 날리면 됨

		printf("\n");
	}

	enter();
}

int remove_tiles(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	int sub_board[MAX_SIZE][MAX_SIZE] = { 0 };
	int i = 0, j = 0;
	int col_check = j, change_tile = 0;

	Copy_Board(board, sub_board, row, col);  // 복사 보드 생성 -> 원본 보드는 데이터 수정용, 복사 보드는 검사(조건 확인)용으로 사용 -> 검사가 끝나면 원본 보드는 우리가 원하는 보드 형태를 가짐

	for (i = 0; i < row - 1;) {  // i를 반복용, j를 검사용으로 사용 
		col_check = Check_Square(sub_board, i, j);  // 현재 위치를 기준으로 정사각형이 되어있는지 확인

		if (j != col_check)  // 정사각형이 아니라면 현재 위치를 옮기기(더 검사할 필요 X)
			j = col_check;

		else {  // 정사각형이면 타일 지우고 검사 위치 옮기기
			change_tile += Delete_Tile(board, i, j);
			j++;
		}

		if (j >= col - 1) {
			j = 0; i++;
		}
	}

	return change_tile;  // 바뀐게 없으면 0 반환 -> 종료 조건으로 이용
}

void update_board(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	int i, j, temp;  // temp: 옮길 숫자가 있는 위치 알려주는 용도

	for (j = 0; j < col; j ++) {  // 세로로 먼저 올리는 알고리즘
		temp = row - 1;

		for (i = row - 1; i >= 0; i--) {  // 세로로 0을 먼저 올림. 바닥에서 위로 검사. i를 고정시켜놓고 temp를 움직임. i는 0을 가리키고, temp가 옮겨야 할 숫자를 찾으면 i와 swap

			if (board[i][j] != 0) {  // 바닥 값이 0이 아니면 올릴 필요 X -> 넘어감
				temp--; continue;
			}

			while (temp >= 0) {  // temp 이동
				if (board[temp][j] == 0)  // temp는 숫자를 찾아야 함
					temp--;
				
				else {
					swap(&board[i][j], &board[temp][j]);
					temp--; break;
				}
			}

			if (temp < 0)  // temp는 밑에서부터 순서대로 모든 숫자를 가리키기에, temp가 0보다 작으면 추가 검사 의미 X
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
	if (board[row][col] == 0) // 현재 위치 타일이 0이면 없는 데이터 -> 검사 X
		return col + 1;

	if (board[row][col] != board[row + 1][col] || board[row][col] != board[row][col + 1])  // 현재 위치 타일과 아래 & 오른쪽이 다른 값이면 오른쪽으로 1칸 이동해서 검사
		return col + 1;

	if (board[row][col] != board[row + 1][col + 1]) // 현재 위치 타일과 대각 위치가 다른 값이면 오른쪽 2칸 이동해서 검사. 1칸 이동해봐야 어차피 다른 값 확정이므로 바로 2칸 이동
		return col + 2;

	return col;
}

int Delete_Tile(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	int i, j, count = 0;

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			if (board[row + i][col + j] != 0) { // 타일을 지우면서 지운 타일 개수 카운트
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