#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXPLAY 10
#define CHIP 50
#define QUESTMARK 63
#define MAX_CARD_NUM 10
#define MIN_CARD_NUM 1
#define NOPAIR 1
#define DOUBLE 2
#define STRAIGHT 3
#define TRIPLE 4
#define CALL 0
#define FOLD -1
#define USER 456
#define COM 123


void print_game_status(int round, int user_chips, int com_chips);
void card_shuffle(int* computer_card, int* shared_card1, int* shared_card2, int* user_card);
void print_card_info(int computer_card, int shared_card1, int shared_card2, int user_card);
int is_valid_num(int a, int b, int num);
int user_turn(int user_chips, int* user_betting_chips, int betted_chips, int turn);
int calc_hand(int card, int shard_card1, int shard_card2);
int computer_turn(int user_hand, int com_chips, int* com_betting_chips, int betted_chips, int turn);
int calc_winner(int computer_card, int shared_card1, int shared_card2, int user_card);

int random(int start, int end);
void print_bet_menu();
void print_bet_status(int user_betting_chips, int com_betting_chips);
int is_valid_choice(int bet_select, int turn);
int control_rand(int hand_rankings, int turn);
int find_min(int card, int shared_card1, int shared_card2);
void print_winner(int winner);

int main() {
	int round = 1;
	int user_chips = CHIP, com_chips = CHIP;
	int user_betting_chips = 1, com_betting_chips = 1;
	int computer_card = -1, shared_card1 = -1, shared_card2 = -1, user_card = -1;
	int winner = COM, bet_choice = 0, turn = 1, keepgoing = 0;

	srand(time(NULL));

	while (1) {
		system("cls");
		print_game_status(round, user_chips, com_chips);
		card_shuffle(&computer_card, &shared_card1, &shared_card2, &user_card);
		print_card_info(computer_card, shared_card1, shared_card2, QUESTMARK);
		
		print_bet_menu();
		do {
			print_bet_status(user_betting_chips, com_betting_chips);
			if (winner == COM || turn != 1) {
				bet_choice = user_turn(user_chips, &user_betting_chips, com_betting_chips, turn);
				print_bet_status(user_betting_chips, com_betting_chips);
				if (bet_choice == FOLD || bet_choice == CALL)
					break;
				turn++;
			}
			bet_choice = computer_turn(calc_hand(user_card, shared_card1, shared_card2), com_chips, &com_betting_chips, user_betting_chips, turn);
			print_bet_status(user_betting_chips, com_betting_chips);
			if (bet_choice == FOLD || bet_choice == CALL)
				break;
			turn++;
		} while (user_betting_chips != com_betting_chips);

		printf("收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收\n\n");
		printf("Betting Finished\n\n");
		print_card_info(computer_card, shared_card1, shared_card2, user_card);
		
		if (bet_choice == FOLD) {
			if (user_betting_chips > com_betting_chips)
				winner = USER;
			else if (user_betting_chips < com_betting_chips)
				winner = COM;
		}
		else
			winner = calc_winner(computer_card, shared_card1, shared_card2, user_card);
		print_winner(winner);
		if (winner == USER) {
			user_chips += com_betting_chips;
			com_chips -= com_betting_chips;
		}
		else {
			com_chips += user_betting_chips;
			user_chips -= user_betting_chips;
		}
		if (user_chips <= 0 || com_chips <= 0 || round > MAXPLAY)
			break;
		printf("Proceed or Not? [Go: 1, End: -1]: ");
		scanf("%d", &keepgoing);
		while (getchar() != '\n');
		if (keepgoing == -1)
			break;
	
		round++;
		turn = 1;
		com_betting_chips = 1;
		user_betting_chips = 1;
	}

	return 0;
}

void print_game_status(int round, int user_chips, int com_chips) {
	printf("旨收收收收收收收收收收收收收收收收收旬\n");
	printf("早 %2d Game Starts! 早\n", round);
	printf("曲收收收收收收收收收收收收收收收收收旭\n\n");

	printf("Chips remaining:\n");
	printf("旨收收收收收收收收收收有收收收收收旬\n");
	printf("早    User  早 %3d 早\n", user_chips);
	printf("早 Computer 早 %3d 早\n", com_chips);
	printf("曲收收收收收收收收收收朴收收收收收旭\n\n");
	return;
}


void card_shuffle(int* computer_card, int* shared_card1, int* shared_card2, int* user_card) {
	*computer_card = random(MIN_CARD_NUM, MAX_CARD_NUM);
	*shared_card1 = random(MIN_CARD_NUM, MAX_CARD_NUM);
	*shared_card2 = random(MIN_CARD_NUM, MAX_CARD_NUM);
	*user_card = random(MIN_CARD_NUM, MAX_CARD_NUM);
	return;
}


void print_card_info(int computer_card, int shared_card1, int shared_card2, int user_card) {
	printf("旨收收收收收收收收收收收收有收收收收收收收收收收收收收收收收收收收收收收收收收有收收收收收收收收收收收收旬\n");
	printf("早  Computer  早      Shared Cards       早    User    早\n");
	printf("曳收收收收收收收收收收收收朱收收收收收收收收收收收收有收收收收收收收收收收收收朱收收收收收收收收收收收收朽\n");
	printf("早 旨收收收收收收收收旬 早 旨收收收收收收收收旬 早 旨收收收收收收收收旬 早 旨收收收收收收收收旬 早\n");
	printf("早 早 Ⅳ      早 早 早 Ⅳ      早 早 早 Ⅳ      早 早 早 Ⅳ      早 早\n");
	printf("早 早        早 早 早        早 早 早        早 早 早        早 早\n");
	if (user_card == QUESTMARK)
		printf("早 早   %2d   早 早 早   %2d   早 早 早   %2d   早 早 早   %2c   早 早\n", computer_card, shared_card1, shared_card2, user_card);
	else
		printf("早 早   %2d   早 早 早   %2d   早 早 早   %2d   早 早 早   %2d   早 早\n", computer_card, shared_card1, shared_card2, user_card);
	printf("早 早        早 早 早        早 早 早        早 早 早        早 早\n");
	printf("早 早      Ⅳ 早 早 早      Ⅳ 早 早 早      Ⅳ 早 早 早      Ⅳ 早 早\n");
	printf("早 曲收收收收收收收收旭 早 曲收收收收收收收收旭 早 曲收收收收收收收收旭 早 曲收收收收收收收收旭 早\n");
	printf("曲收收收收收收收收收收收收朴收收收收收收收收收收收收朴收收收收收收收收收收收收朴收收收收收收收收收收收收旭\n\n");
}


int is_valid_num(int a, int b, int num) {
	if (a <= num && num <= b)
		return 1;
	return 0;
}


int user_turn(int user_chips, int* user_betting_chips, int betted_chips, int turn) {
	int bet_choice = -1, raise = 0;

	do {
		printf("USER 早 [Call: 1 | Raise: 2 | Fold : 3]: ");
		scanf("%d", &bet_choice);
		while (getchar() != '\n');
	} while (is_valid_choice(bet_choice, turn) == 0);

	switch (bet_choice) {
	case 1:
		if (betted_chips >= user_chips)
			*user_betting_chips = user_chips;
		*user_betting_chips = betted_chips;
		return CALL;
	case 2:
		*user_betting_chips = betted_chips;
		printf("USER 早 [Input number of chips for Raise]: ");
		scanf("%d", &raise);
		while (getchar() != '\n');
		if (*user_betting_chips + raise > user_chips)
			*user_betting_chips = user_chips;
		else
			*user_betting_chips += raise;
		printf("\n");
		return raise;
	}
	printf("\n");
	return FOLD;
}


int calc_hand(int card, int shared_card1, int shared_card2) {
	int min = find_min(card, shared_card1, shared_card2);
	if (card == shared_card1 && card == shared_card2)
		return TRIPLE;
	else if (card == shared_card1 || card == shared_card2)
		return DOUBLE;
	else if (3 * (min + 1) == card + shared_card1 + shared_card2)
		return STRAIGHT;
	else
		return NOPAIR;
}


int computer_turn(int user_hand, int com_chips, int* com_betting_chips, int betted_chips, int turn) {
	int raise = 0;

	switch (control_rand(user_hand, turn)) {
	case CALL:
		if (turn != 1) {
			if (betted_chips >= com_chips)
				*com_betting_chips = com_chips;
			else
				*com_betting_chips = betted_chips;
			printf("COM 早 Call\n\n");
			return CALL;
		}
	case 1:
		raise = random(1, 5);
		*com_betting_chips = betted_chips;
		if (turn == 1) {
			*com_betting_chips += 1;
			raise = 1;
		}
		if (*com_betting_chips + raise > com_chips) {
			if (*com_betting_chips > com_chips)
				raise = *com_betting_chips - com_chips;
			else
				raise = com_chips - *com_betting_chips;
			*com_betting_chips = com_chips;
		}
		if (turn != 1 && ( * com_betting_chips + raise <= com_chips))
			*com_betting_chips += raise;
		printf("COM 早 Raise, %+d\n\n", raise);
		return raise;
	}
	printf("COM 早 FOLD\n\n");
	return FOLD;
}


int calc_winner(int computer_card, int shared_card1, int shared_card2, int user_card) {
	int com_hand = 0, user_hand = 0;

	com_hand = calc_hand(computer_card, shared_card1, shared_card2);
	user_hand = calc_hand(user_card, shared_card1, shared_card2);
	if (com_hand > user_hand)
		return COM;
	else if (com_hand == user_hand && computer_card > user_card)
		return COM;
	else
		return USER;
}

int random(int start, int end) {
	return rand() % (end - start + 1) + start;
}

void print_bet_menu() {
	printf("旨收收收收收收收收收收收收收收收旬\n");
	printf("早    Betting    早\n");
	printf("曲收收收收收收收收收收收收收收收旭\n\n");
	return;
}

void print_bet_status(int user_betting_chips, int com_betting_chips) {
	printf("旨收收收收收收收收收收收收收收收收收收收收旬\n");
	printf("早 User: %2d 早 Com: %2d 早\n", user_betting_chips, com_betting_chips);
	printf("曲收收收收收收收收收收收收收收收收收收收收旭\n\n");
}

int is_valid_choice(int bet_select, int turn) {
	if ((is_valid_num(1, 3, bet_select) == 0) || (turn == 1 && bet_select == 1)) {
		printf("USER 早 [Invalid input]\n");
		return 0;
	}
	return 1;
}

int control_rand(int hand_rankings, int turn) {
	int ref = random(1, 100);

	if (hand_rankings == NOPAIR) {
		if (turn == 1)
			return 1;
		if (ref <= 50)
			return 1;
		else
			return CALL;
	}
	else {
		if (ref <= 70)
			return FOLD;
		else if (turn == 1)
			return 0;
			return CALL;
	}
}

int find_min(int card, int shared_card1, int shared_card2) {
	int min = card;
	if (min > shared_card1)
		min = shared_card1;
	if (min > shared_card2)
		min = shared_card2;
	return min;
}

void print_winner(int winner) {
	printf("旨收收收收收收收收收收收收收收收收收收收旬\n");
	if(winner == COM)
		printf("早   Computer win!   早\n");
	else
		printf("早     User win!     早\n");
	printf("曲收收收收收收收收收收收收收收收收收收收旭\n\n");
}