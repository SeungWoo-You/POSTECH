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
	float input_top_number = 0;		// invalid input value�� ���� ���� 0���� �ʱ�ȭ, �񱳸� ���� float Ÿ�� ����
	int floor_ = 0, floor_digit, i, floor_count = 0, pole1 = 0, pole2 = 0, pole3 = 0;		// i �� iteration ���� ǥ��

	printf("�ϳ����� ž �� ���� �����ּ���(1������ 5�� ����): ");

	while (1) {
		scanf("%f", &input_top_number);

		if (input_top_number < 1 || input_top_number > 5 || (input_top_number != (int)input_top_number)) {		// 1-1 error check
			printf("�߸��� �Է��Դϴ�. 1���� 5 ������ ���ڸ� �Է����ּ���: ");
			
			continue;
		}

		floor_ = (int)input_top_number;		// ������ ����� floor�� ����

		break;
	}
	
	for (i = floor_; i > 0; i--) {		// 1-2 decide how to calc hanoi
		floor_digit = Pow_10(i - 1);
		pole1 += (++floor_count) * floor_digit;
	}

	main_hanoi(floor_, pole1, pole2, pole3);
	printf("���� ��� �ٸ� ������� �Ű������ϴ�. ������ �����մϴ�.\n");

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

		printf("1.���� �ű� ����� �������ּ���.(1-3): ");		// 2-1-1 choose from pole
		while (1) {
			scanf("%f", &input_from_pole);

			if (input_from_pole < 1 || input_from_pole > 3 || (input_from_pole != (int)input_from_pole)) {
				printf("1.�߸��� �Է��Դϴ�. 1���� 3 ������ ���ڸ� �Է����ּ���: ");
				continue;
			}

			from_pole = Find_Choose_Pole((int)input_from_pole, &pole1, &pole2, &pole3);

			if (check_not_empty(*from_pole) == 0) {
					printf("1.�ش� ����� ����ֽ��ϴ�. ������� ���� ����� �������ּ���: ");
					continue;
			}

			break;
		}

		printf("2.���� �Ű��� ����� �������ּ���.(1-3): ");		// 2-1-2 choose to pole
		while (1) {
			scanf("%f", &input_to_pole);

			if (input_to_pole < 1 || input_to_pole > 3 || (input_to_pole != (int)input_to_pole)) {
				printf("2.�߸��� �Է��Դϴ�. 1���� 3 ������ ���ڸ� �Է����ּ���: ");
				continue;
			}

			to_pole = Find_Choose_Pole((int)input_to_pole, &pole1, &pole2, &pole3);

			if (play_hanoi(from_pole, to_pole)) {		// 2-1-3 move
				printf("2.�ϳ����� ž ��Ģ�� ��߳��ϴ�. �ٸ� ����� �������ּ���: ");
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
	printf("[��� 1]\n");
	print_one_pole(floor_, pole1);
	printf("\n");

	printf("[��� 2]\n");
	print_one_pole(floor_, pole2);
	printf("\n");

	printf("[��� 3]\n");
	print_one_pole(floor_, pole3);
}

void print_one_pole(int floor_, int pole) {
	int i, j, floor_digit = 1;

	if (check_not_empty(pole) == 0) {
		printf("Pole is empty\n");
		return;
	}

	floor_digit = NumberOfDigits(pole);


	for (i = floor_digit; i > 0; i--) {		// ���������� ����
		for (j = 0; j < pole / Pow_10(i - 1); j++)		// ���� ��ŭ �׸� ���
			printf("��");
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