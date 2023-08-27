#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXLETTER 20		// ���ڼ� ���߱� ���� ��ũ��
#define MAXPLAY 9		// �÷��� Ƚ�� ���� �� ��ũ��

void draw_line(int num, char print_char);		// �� �׸���. �Ϲ����� ���� ���
void draw_line(int num, char* print_char);		// �� �׸���. �����ڵ� ���� ���
void draw_line(int num, char print_char, int type);		// �� �׸���. \n���� ���·� ���
void draw_line(int num, char* print_char, int type);		// �� �׸���. \n ���� ����, �����ڵ� ���� ���
void print_marble_status(int user_marble, int com_marble);		//���� ��Ȳ ���
int get_rand_num(int start, int end);		// ���� �� �ޱ�
int get_user_guess();		// ����� ���� �� �Է� �ޱ�
int is_valid_num(int min_num, int max_num, int num);		// ��ȿ �� Ȯ�ο�
void change_marble_status(int* winner, int* loser, int marble);		// ���� ���
int get_user_choice(int marble);	// ���� ���� �Է� �ޱ�
int User_Input(int start, int end);	// ���� ��ǲ ��Ʈ�� �Լ�. 0 ~ 2 ��Ʈ��
int User_Input(int start, int end, int type);	// ���� ��ǲ ��Ʈ�� �Լ�. 1 ~ (���� ��) ��Ʈ��
void Play_Game(int* user, int* com, int who_turn);		// ���� ����

// draw_line ���� �Լ� ���� �� ����� �Ȱ� �����ؼ� 2�г⶧ ���� �����ε� ���� ���. (User_Input���� �����ε� ����)
// ���� ������ �����ε� ���ϰ� �Լ� ������ ���� OK

int main() {
	int game_round = 1, play_count = 0, win_count = 0;
	int	user_marble = 20, com_marble = 20;
	char play_again;
	char square[3] = "��";		// �����ڵ�� 2bytes�� 1byte�� char�� ��� �� ���� �߻� -> ���ڿ� ���·� ���� ó��

	srand(time(NULL));

	printf("[CESD101 Assignment 2]\n");		// 1. �ʱ�ȭ�� ���
	draw_line(MAXLETTER, square);
	draw_line(2, square, NULL); draw_line(MAXLETTER - 4, ' ', NULL); draw_line(2, square);
	draw_line(2, square, NULL); draw_line(5, ' ', NULL); printf("Marble  Game"); draw_line(5, ' ', NULL); draw_line(2, square);
	draw_line(2, square, NULL); draw_line(MAXLETTER - 4, ' ', NULL); draw_line(2, square);
	draw_line(MAXLETTER, square);
	printf("\n");
	printf(" Welcome to the Marble Game! Have fun~!\n");
	draw_line(MAXLETTER, square);
	printf("\n");

	while (play_count < MAXPLAY) {		// 2 ~ 4 �ݺ� ����
		++play_count;
		printf("[Game #%d]\n", play_count);		// 2. ���ӽ���ȭ�� ��� & 2-1. ���° ����?
		print_marble_status(user_marble, com_marble);

		while (user_marble && com_marble) {		// 3 ~ 4 �ݺ� ����
			Play_Game(&user_marble, &com_marble, game_round);	// 3. ��ǻ�� ���� ����� & 4. ���� ���� �����
			++game_round;

			print_marble_status(user_marble, com_marble);
		}

		printf("GAME OVER!\n");		// 5. ���� ���� ���

		if (user_marble == 0)
			printf("You have lost to the computer :(\n\n");

		else if (com_marble == 0) {
			printf("You have won!:)\n\n");
			++win_count;
		}
		printf("Would you like to play again?\n");
		printf("Type 'y' to play: \n");
		while (getchar() != '\n');		// �Է� ���� ����
		scanf("%c", &play_again);

		if (play_count == 9) {		// 9���� �ϸ� ����� �Է� ���� �ٷ� ������
			printf("You play already %d times! Program ends...\n\n", MAXPLAY);
			break;
		}

		if (play_again == 'y') {		// �ٽ� ����
			user_marble = 20;
			com_marble = 20;
			game_round = 1;

			continue;
		}

		else
			break;
	}
		
	draw_line(MAXLETTER, square); printf("\n");		// 6. ���α׷� ����
	printf("You have won %d games out of %d!\n", win_count, play_count);
	printf("Thank you for playing! Bye~!");

	return 0;
}


void draw_line(int num, char print_char) {
	int i;
	
	for (i = 0; i < num * 2; i++)		// ���ϴ� ���� �� ��ŭ ���. �׷��� char�� 1byte�� 1ĭ ���, �����ڵ�� 2ĭ ��� -> ĭ �� ���߷��� num * 2��ŭ ����
		printf("%c", print_char);
	
	printf("\n");
}

void draw_line(int num, char* print_char) {
	int i;

	for (i = 0; i < num; i++)
		printf("%c%c", print_char[0], print_char[1]);		// �̷��� ��� 2bytes ���� ��� ����

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
	char black_circle[3] = "��", white_circle[3] = "��";		// �굵 2bytes ���ڶ� ���ڿ��� ǥ�� 

	draw_line(MAXLETTER, '-');
	printf("USER: �� (%2d)\n", user_marble);
	printf("COMP: �� (%2d)\n", com_marble);

	if (user_marble <= MAXLETTER) {		// 2-2. ���� ���
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

int get_user_guess() {		// 3-1. ���� �� �Է�
	int choice;

	printf("It's your turn to guess!\n\n");
	printf("Guess 0, 1, or 2: ");

	choice = User_Input(0, 2);		// ���⼭ is_valid_num�� �ҷ��� ���� ó��

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

int get_user_choice(int marble) {		// 4-1. ���� ���� �Է�
	int choice;

	printf("It s the computer's turn to guess!\n");
	printf("How many marbles would you like to hide? ");

	choice = User_Input(1, marble, NULL);
	printf("You hid %d marbles\n\n", choice);

	return choice;
}

int User_Input(int start, int end) {		// ���� �ȸ��� ������ ���� ���ϰ� �Ϸ��� ���� �Լ�
	int choice;

	while (scanf("%d", &choice) != 1 || !is_valid_num(start, end, choice)) {
		while (getchar() != '\n');

		printf("That is a wrong input!\n");
		printf("Please guess 0, 1, or 2: ");
	}

	return choice;
}

int User_Input(int start, int end, int type) {		// �굵 ������
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
	
	if (who_turn % 2 == 1) {		// 3. ��ǻ�� ���� �����
		hide_marble = get_rand_num(1, *com);
		hide_number = hide_marble % 3;

		if (get_user_guess() == hide_number) {		// 3-2. ��� Ȯ��
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

	else {		// 4. ���� ���� �����
		hide_marble = get_user_choice(*user);		// 4-1. ���� ���� �Է�
		hide_number = hide_marble % 3;
		com_choice = get_rand_num(1, *com);		// 4-2. ��ǻ�� ���� �� ����

		if (com_choice % 3 == hide_number) {		// 4-3. ��� Ȯ��
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