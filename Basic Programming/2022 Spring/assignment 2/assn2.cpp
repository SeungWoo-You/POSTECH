#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXLETTER 20		// 글자수 맞추기 위한 매크로
#define MAXPLAY 9		// 플레이 횟수 조절 용 매크로

void draw_line(int num, char print_char);		// 줄 그리기. 일반적인 문자 출력
void draw_line(int num, char* print_char);		// 줄 그리기. 유니코드 문자 출력
void draw_line(int num, char print_char, int type);		// 줄 그리기. \n없는 형태로 출력
void draw_line(int num, char* print_char, int type);		// 줄 그리기. \n 없는 형태, 유니코드 문자 출력
void print_marble_status(int user_marble, int com_marble);		//구슬 현황 출력
int get_rand_num(int start, int end);		// 랜덤 값 받기
int get_user_guess();		// 사용자 추측 값 입력 받기
int is_valid_num(int min_num, int max_num, int num);		// 유효 값 확인용
void change_marble_status(int* winner, int* loser, int marble);		// 구슬 계산
int get_user_choice(int marble);	// 유저 선택 입력 받기
int User_Input(int start, int end);	// 유저 인풋 컨트롤 함수. 0 ~ 2 컨트롤
int User_Input(int start, int end, int type);	// 유저 인풋 컨트롤 함수. 1 ~ (구슬 수) 컨트롤
void Play_Game(int* user, int* com, int who_turn);		// 게임 진행

// draw_line 관련 함수 여러 개 만들기 싫고 복잡해서 2학년때 배우는 오버로드 개념 사용. (User_Input에도 오버로드 적용)
// 굳이 저렇게 오버로드 안하고 함수 여러개 만들어도 OK

int main() {
	int game_round = 1, play_count = 0, win_count = 0;
	int	user_marble = 20, com_marble = 20;
	char play_again;
	char square[3] = "▩";		// 유니코드는 2bytes라서 1byte인 char로 계산 시 오류 발생 -> 문자열 형태로 만들어서 처리

	srand(time(NULL));

	printf("[CESD101 Assignment 2]\n");		// 1. 초기화면 출력
	draw_line(MAXLETTER, square);
	draw_line(2, square, NULL); draw_line(MAXLETTER - 4, ' ', NULL); draw_line(2, square);
	draw_line(2, square, NULL); draw_line(5, ' ', NULL); printf("Marble  Game"); draw_line(5, ' ', NULL); draw_line(2, square);
	draw_line(2, square, NULL); draw_line(MAXLETTER - 4, ' ', NULL); draw_line(2, square);
	draw_line(MAXLETTER, square);
	printf("\n");
	printf(" Welcome to the Marble Game! Have fun~!\n");
	draw_line(MAXLETTER, square);
	printf("\n");

	while (play_count < MAXPLAY) {		// 2 ~ 4 반복 실행
		++play_count;
		printf("[Game #%d]\n", play_count);		// 2. 게임시작화면 출력 & 2-1. 몇번째 게임?
		print_marble_status(user_marble, com_marble);

		while (user_marble && com_marble) {		// 3 ~ 4 반복 실행
			Play_Game(&user_marble, &com_marble, game_round);	// 3. 컴퓨터 구슬 숨기기 & 4. 유저 구슬 숨기기
			++game_round;

			print_marble_status(user_marble, com_marble);
		}

		printf("GAME OVER!\n");		// 5. 게임 종료 출력

		if (user_marble == 0)
			printf("You have lost to the computer :(\n\n");

		else if (com_marble == 0) {
			printf("You have won!:)\n\n");
			++win_count;
		}
		printf("Would you like to play again?\n");
		printf("Type 'y' to play: \n");
		while (getchar() != '\n');		// 입력 버퍼 비우기
		scanf("%c", &play_again);

		if (play_count == 9) {		// 9판을 하면 사용자 입력 없이 바로 끝내기
			printf("You play already %d times! Program ends...\n\n", MAXPLAY);
			break;
		}

		if (play_again == 'y') {		// 다시 시작
			user_marble = 20;
			com_marble = 20;
			game_round = 1;

			continue;
		}

		else
			break;
	}
		
	draw_line(MAXLETTER, square); printf("\n");		// 6. 프로그램 종료
	printf("You have won %d games out of %d!\n", win_count, play_count);
	printf("Thank you for playing! Bye~!");

	return 0;
}


void draw_line(int num, char print_char) {
	int i;
	
	for (i = 0; i < num * 2; i++)		// 원하는 문자 수 만큼 출력. 그런데 char은 1byte라서 1칸 출력, 유니코드는 2칸 출력 -> 칸 수 맞추려고 num * 2만큼 진행
		printf("%c", print_char);
	
	printf("\n");
}

void draw_line(int num, char* print_char) {
	int i;

	for (i = 0; i < num; i++)
		printf("%c%c", print_char[0], print_char[1]);		// 이렇게 써야 2bytes 문자 출력 가능

	printf("\n");
}

void draw_line(int num, char print_char, int type) {
	int i;

	for (i = 0; i < num * 2; i++)
		printf("%c", print_char);
}

void draw_line(int num, char* print_char, int type) {
	int i;

	for (i = 0; i < num; i++)
		printf("%c%c", print_char[0], print_char[1]);
}

void print_marble_status(int user_marble, int com_marble) {
	char black_circle[3] = "●", white_circle[3] = "○";		// 얘도 2bytes 문자라서 문자열로 표현 

	draw_line(MAXLETTER, '-');
	printf("USER: ○ (%2d)\n", user_marble);
	printf("COMP: ● (%2d)\n", com_marble);

	if (user_marble <= MAXLETTER) {		// 2-2. 구슬 출력
		draw_line(user_marble, white_circle, NULL);
		draw_line(MAXLETTER - user_marble, black_circle);
		draw_line(MAXLETTER, black_circle);
	}

	else {
		draw_line(MAXLETTER, white_circle);
		draw_line(user_marble - MAXLETTER, white_circle, NULL);
		draw_line(com_marble, black_circle);
	}

	draw_line(MAXLETTER, '-');
	printf("\n");
}

int get_rand_num(int start, int end) {
	return rand() % (end - start + 1) + start;
}

int get_user_guess() {		// 3-1. 추측 값 입력
	int choice;

	printf("It's your turn to guess!\n\n");
	printf("Guess 0, 1, or 2: ");

	choice = User_Input(0, 2);		// 여기서 is_valid_num을 불러서 오류 처리

	printf("You guessed %d.\n\n", choice);

	return choice;
}

int is_valid_num(int min_num, int max_num, int num) {
	if (num >= min_num && num <= max_num)
		return 1;

	return 0;
}

void change_marble_status(int* winner, int* loser, int marble) {
	if (marble <= *loser) {
		*winner += marble;
		*loser -= marble;
	}

	else {
		*winner += *loser;
		*loser = 0;
	}
}

int get_user_choice(int marble) {		// 4-1. 숨길 구슬 입력
	int choice;

	printf("It s the computer's turn to guess!\n");
	printf("How many marbles would you like to hide? ");

	choice = User_Input(1, marble, NULL);
	printf("You hid %d marbles\n\n", choice);

	return choice;
}

int User_Input(int start, int end) {		// 굳이 안만들어도 되지만 관리 편하게 하려고 만든 함수
	int choice;

	while (scanf("%d", &choice) != 1 || !is_valid_num(start, end, choice)) {
		while (getchar() != '\n');

		printf("That is a wrong input!\n");
		printf("Please guess 0, 1, or 2: ");
	}

	return choice;
}

int User_Input(int start, int end, int type) {		// 얘도 관리용
	int choice;

	while (scanf("%d", &choice) != 1 || !is_valid_num(start, end, choice)) {
		while (getchar() != '\n');

		printf("That is a wrong input!\n");
		printf("Please select a number between 1 and %d: ", end);
	}

	return choice;
}

void Play_Game(int* user, int* com, int who_turn) {
	int hide_marble, hide_number, com_choice;
	int* winner = user, * loser = com;
	
	if (who_turn % 2 == 1) {		// 3. 컴퓨터 구슬 숨기기
		hide_marble = get_rand_num(1, *com);
		hide_number = hide_marble % 3;

		if (get_user_guess() == hide_number) {		// 3-2. 결과 확인
			printf("You guessed correctly!\n");
			printf("The computer hid %d marbles.\n\n", hide_marble);

			winner = user;
			loser = com;
		}

		else {
			printf("You guessed incorrectly...\n");
			printf("The computer hid %d marbles.\n", hide_marble);
			printf("The correct guess would have been %d.\n\n", hide_number);

			winner = com;
			loser = user;
		}
	}

	else {		// 4. 유저 구슬 숨기기
		hide_marble = get_user_choice(*user);		// 4-1. 숨길 구슬 입력
		hide_number = hide_marble % 3;
		com_choice = get_rand_num(1, *com);		// 4-2. 컴퓨터 추측 값 생성

		if (com_choice % 3 == hide_number) {		// 4-3. 결과 확인
			printf("The computer guessed %d.\n", com_choice % 3);
			printf("The computer guessed correctly...\n\n");

			winner = com;
			loser = user;
		}

		else {
			printf("The computer guessed %d.\n", com_choice % 3);
			printf("The computer guessed incorrectly!\n");
			printf("The correct guess would have been %d.\n\n", hide_number);

			winner = user;
			loser = com;
		}
	}

	change_marble_status(winner, loser, hide_marble);
}