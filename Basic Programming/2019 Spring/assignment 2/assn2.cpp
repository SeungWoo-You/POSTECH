#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main_hanoi(int floor_, int pole1, int pole2, int pole3);
int Pow_10(int n);
void print_divider(int num);
int NumberOfDigits(int num);
void print_poles(int floor_, int pole1, int pole2, int pole3);
void print_one_pole(int floor_, int pole);
int play_hanoi(int* from_pole, int* to_pole);
int check_valid_move(int from_pole_top, int to_pole);
int check_not_empty(int pole);
int* Find_Choose_Pole(int input_pole_num, int* pole1, int* pole2, int* pole3);
int check_finish(int floor_, int digit);

int main() {
	float input_top_number = 0;		// invalid input value를 막기 위해 0으로 초기화, 비교를 위해 float 타입 설정
	int floor_ = 0, floor_digit, i, floor_count = 0, pole1 = 0, pole2 = 0, pole3 = 0;		// i 는 iteration 줄임 표기

	printf("하노이의 탑 층 수를 정해주세요(1층부터 5층 사이): ");

	while (1) {
		scanf("%f", &input_top_number);

		if (input_top_number < 1 || input_top_number > 5 || (input_top_number != (int)input_top_number)) {		// 1-1 error check
			printf("잘못된 입력입니다. 1부터 5 사이의 숫자를 입력해주세요: ");
			
			continue;
		}

		floor_ = (int)input_top_number;		// 실제로 사용할 floor값 저장

		break;
	}
	
	for (i = floor_; i > 0; i--) {		// 1-2 decide how to calc hanoi
		floor_digit = Pow_10(i - 1);
		pole1 += (++floor_count) * floor_digit;
	}

	main_hanoi(floor_, pole1, pole2, pole3);
	printf("블럭이 모두 다른 기둥으로 옮겨졌습니다. 퍼즐을 종료합니다.\n");

	return 0;
}

void main_hanoi(int floor_, int pole1, int pole2, int pole3) {
	float input_from_pole = 0, input_to_pole = 0;
	int temp_pole = 0, step = 1;
	int* from_pole = 0, * to_pole = 0;

	while (1) {
		print_divider(40);
		print_divider(40);
		printf("%d %d %d\n", pole1, pole2, pole3);

		printf(">>>Step %d\n\n", step);
		print_poles(floor_, pole1, pole2, pole3);		// 2-2 print the pole

		print_divider(40);
		print_divider(40);

		if (check_finish(floor_, NumberOfDigits(pole2)) || check_finish(floor_, NumberOfDigits(pole3)))		// 3 check finish
			break;

		printf("1.블럭을 옮길 기둥을 선택해주세요.(1-3): ");		// 2-1-1 choose from pole
		while (1) {
			scanf("%f", &input_from_pole);

			if (input_from_pole < 1 || input_from_pole > 3 || (input_from_pole != (int)input_from_pole)) {
				printf("1.잘못된 입력입니다. 1부터 3 사이의 숫자를 입력해주세요: ");
				continue;
			}

			from_pole = Find_Choose_Pole((int)input_from_pole, &pole1, &pole2, &pole3);

			if (check_not_empty(*from_pole) == 0) {
					printf("1.해당 기둥이 비어있습니다. 비어있지 않은 기둥을 선택해주세요: ");
					continue;
			}

			break;
		}

		printf("2.블럭이 옮겨질 기둥을 선택해주세요.(1-3): ");		// 2-1-2 choose to pole
		while (1) {
			scanf("%f", &input_to_pole);

			if (input_to_pole < 1 || input_to_pole > 3 || (input_to_pole != (int)input_to_pole)) {
				printf("2.잘못된 입력입니다. 1부터 3 사이의 숫자를 입력해주세요: ");
				continue;
			}

			to_pole = Find_Choose_Pole((int)input_to_pole, &pole1, &pole2, &pole3);

			if (play_hanoi(from_pole, to_pole)) {		// 2-1-3 move
				printf("2.하노이의 탑 규칙에 어긋납니다. 다른 기둥을 선택해주세요: ");
					continue;
			}

			break;
		}

		system("cls");
		printf("Block Moved!\n");
		step++;
	}

	return;
}

int Pow_10(int n) {
	return (int)pow(10, n);
}

void print_divider(int num) {
	int i;

	for (i = 0; i < num; i++) {
		printf("=");
	}
	printf("\n");

	return;
}

int NumberOfDigits(int num) {
	if (num <= 0)
		return 0;

	return (int)floor(log10(num) + 1);
}

void print_poles(int floor_, int pole1, int pole2, int pole3) {
	printf("[기둥 1]\n");
	print_one_pole(floor_, pole1);
	printf("\n");

	printf("[기둥 2]\n");
	print_one_pole(floor_, pole2);
	printf("\n");

	printf("[기둥 3]\n");
	print_one_pole(floor_, pole3);
}

void print_one_pole(int floor_, int pole) {
	int i, j, floor_digit = 1;

	if (check_not_empty(pole) == 0) {
		printf("Pole is empty\n");
		return;
	}

	floor_digit = NumberOfDigits(pole);


	for (i = floor_digit; i > 0; i--) {		// 위에서부터 시작
		for (j = 0; j < pole / Pow_10(i - 1); j++)		// 숫자 만큼 네모 출력
			printf("□");
		pole -= pole / Pow_10(i - 1) * Pow_10(i - 1);

		printf("\n");
	}
}

int play_hanoi(int* from_pole, int* to_pole) {
	int floor_digit_from = NumberOfDigits(*from_pole), floor_digit_to = NumberOfDigits(*to_pole), block = *from_pole / Pow_10(floor_digit_from - 1);

	if (from_pole == to_pole)
		return 0;

	if (check_valid_move(block, *to_pole)) {		// 2-1-2 case2: check the rule
		*from_pole -= block * Pow_10(floor_digit_from - 1);

		if (NumberOfDigits(*to_pole))
			*to_pole += block * Pow_10(floor_digit_to);

		else
			*to_pole += block;

		return 0;
	}

	else
		return 1;
}

int check_valid_move(int from_pole_top, int to_pole) {
	int block = 0;

	if (NumberOfDigits(to_pole) == 0)
		return 1;

	block = to_pole / Pow_10(NumberOfDigits(to_pole) - 1);

	if (from_pole_top <= block)
		return 1;

	return 0;
}

int check_not_empty(int pole) {
	if (pole == 0)
		return 0;

	return 1;
}

int* Find_Choose_Pole(int input_pole_num, int* pole1, int* pole2, int* pole3) {
	switch (input_pole_num) {
	case 1:
		return pole1;
	case 2:
		return pole2;
	case 3:
		return pole3;
	default:
		exit(0);
	}
}

int check_finish(int floor_, int digit) {
	if (floor_ == digit)
		return 1;

	return 0;
}