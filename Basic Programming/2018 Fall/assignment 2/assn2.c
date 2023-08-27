#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

int show_menu();
void line(int);
void print_tutorial(); // Ʃ�丮�� ǥ�� 
int start_game(); // ���� ���� 
void end(); // ���� ���� 
void error(); // �߸��� �� �Է½� ���
int card_select(); // Ȳ�� or �뿹 �� ���� 
void show_table(int, int); // ī�� ��� ��� 
int get_user_card(int, int, int, int, int); // ���� ī�� ���� 
int get_computer_card(int); // ��ǻ�� ī�� ���� ���� 
int compare_card(int, int, int); // �� ī�� �� 
void choice_result(int); // ���� ��� ǥ�� 
void stat(int, int, int); // ���(�� ��, ����) ǥ�� 
void calc_score(int, int*, int*); // ���� ��� 
void print_score(int, int*, int*); // ���� ǥ�� 
void final_result(int); // ���� ��� ǥ��

int main() {
	int menu;
	srand(time(NULL));

	while (1) {
		menu = show_menu();
		switch (menu) {
		case 1:
			print_tutorial();
			break;
		case 2:
			start_game();
			break;
		case 3:
			end();
			break;
		default:
			error();
		}
	}
	return 0;
}

int show_menu() {
	int select;

	system("cls");
	line(10);
	printf(" E - Card ");
	line(14);
	printf("\n\n");
	printf("	1. ���� ����	\n\n");
	printf("	2. ���� ����	\n\n");
	printf("	3. ���� ����	\n\n");
	line(34);
	printf("\n\n");
	printf("�Է� : ");

	scanf("%d", &select);
	return select;
}

void line(int a) {
	int i;
	for (i = 0; i < a; i++) {
		printf("*"); //���ϴ� ����(a)��ŭ �� ǥ�� 
	}
}

void print_tutorial() {
	system("cls");
	line(20);
	printf("	E - Card ���� ����	");
	line(28);
	printf("\n\n");

	printf("�� ������ ���ڹ��÷� ī������ E - Card ������ ������� �Ѵ�.\n\n");
	printf("1. �� �÷��̾�� Ȳ���п� �뿹�� �� �ϳ��� �и� ���� �����Ѵ�.\n");
	printf("   Ȳ���� = Ȳ�� ī�� 1�� + �ù� ī�� 4��\n");
	printf("   �뿹�� = �뿹 ī�� 1�� + �ù� ī�� 4��\n\n");
	printf("2. �� ����ڴ� �� �ϸ��� �� ���� ī�带 �Ⱥ��̰� ���а�, �Բ� �����´�.\n\n");
	printf("3. �Ʒ��� ī�� �� �� ���迡 ���� ���и� �����Ѵ�.\n");
	printf("   ��, �� �÷��̾ �ù� ī�带 �´ٸ�, ���ºη� ó���Ѵ�.\n\n");
	printf("4. ���о��� ī��� �Ҹ�ǰ� ���а� ������ ������ 2�� ���ư� �ݺ��Ѵ�.\n\n");
	printf("�󼺰��� : Ȳ�� > �ù� > �뿹 > Ȳ��\n\n");

	line(76);
	printf("\n");
	printf("�޴��� ���ư����� EnterŰ�� �Է��ϼ���...\n");
	while (getchar() != '\n'); //Ű���� ���� �� �ʱ�ȭ 
	getchar(); 
	// Enter ���� ���� ���� �Է��� ��� Ű���� ���۸� �ʱ�ȭ ���� �����߻��� ���� ���� 
}

int start_game() {
	int i, csel, getuc, getcc, choice;
	int turn = 1, userscore = 0, comscore = 0;
	char answer;

	while (userscore < 2000 && comscore < 2000) { //�� ���� 2000�̸��� �� �۵��ϰ� �� 2000�� ������ �������� ���ư����� �� 
		system("cls");
		stat(turn, comscore, userscore);
		printf("\n\n\n");
		printf("[ī�� �� ����]\n");
		csel = card_select();
		switch (csel) {
		case 0:
			getuc = get_user_card(0, 4, turn, comscore, userscore);
			break;
		case 1:
			getuc = get_user_card(1, 4, turn, comscore, userscore);
			break;
		default:
			error();
			continue;
		}
		getcc = get_computer_card(4);
		choice = compare_card(csel, getuc, getcc);
		choice_result(choice);

		for (i = 3; i > -1; i--) { // ���ºν� �ºθ� ���� ���� 
			if (choice == -1) {
				printf("���� �� (EnterŰ�� �Է��ϼ���...)");
				while (getchar() != '\n');
				getchar();
				getuc = get_user_card(csel, i, turn, comscore, userscore);
				getcc = get_computer_card(i);
				choice = compare_card(csel, getuc, getcc); //������ ����� ǥ���ߴ��� ȭ���� �����⿡ �ٽ� ǥ�� 
				choice_result(choice);
			}
			else // ���º� �ƴ� �� 
				break;
		}
		print_score(choice, &userscore, &comscore);

		if (userscore < 2000 && comscore < 2000) { //���� �Ǵ� ��ǻ���� ������ 2000 �̸��� ���� ǥ�õǵ��� �� 
			printf("������ ����Ͻðڽ��ϱ�? (y/n) : ");
			while (getchar() != '\n');
			answer = getchar();
			if (answer == 'y' || answer == 'Y') {
				system("cls");
				turn += 1;
			}
			if (answer == 'n' || answer == 'N') {
				system("cls");
				break;
			}
		}
	}

	if (answer == 'n' || answer == 'N') // n�� �������� �� ������� ǥ�� ���� main���� ���ư��� ���� 
		return 0;

	final_result(userscore);

	return 0;
}

void end() {
	printf("���α׷��� �����մϴ�...");
	exit(0); // ���α׷� ����
}

void error() {
	printf("�ùٸ� �޴��� �����ϼ���...");
	Sleep(1000);
	system("cls");
}

int card_select() {
	int select;
	line(30);
	printf("\n\n");
	printf("0. Ȳ���� (Ȳ�� 1��, �ù� 4��)\n\n");
	printf("1. �뿹�� (�뿹 1��, �ù� 4��)\n\n");
	line(30);
	printf("\n\n");
	printf("�Է� : ");
	scanf("%d", &select);
	return select;
}

void show_table(int select, int citynum) {

	printf("\n\n");
	line(50);
	printf("\n");

	if (select == 0 && citynum == 4) { //Ȳ�� & �ù� 4�� 
		printf("������	������	������	������	������\n");
		printf("��?��	��?��	��?��	��?��	��?��\n");
		printf("������	������	������	������	������\n\n\n");
		printf("������	������	������	������	������\n");
		printf("��E��	��C��	��C��	��C��	��C��\n");
		printf("������	������	������	������	������\n");
	}

	if (select == 0 && citynum == 3) { //Ȳ�� & �ù� 3��
		printf("������	������	������	������\n");
		printf("��?��	��?��	��?��	��?��\n");
		printf("������	������	������	������\n\n\n");
		printf("������	������	������	������\n");
		printf("��E��	��C��	��C��	��C��\n");
		printf("������	������	������	������\n");
	}

	if (select == 0 && citynum == 2) { //Ȳ�� & �ù� 2��
		printf("������	������	������\n");
		printf("��?��	��?��	��?��\n");
		printf("������	������	������\n\n\n");
		printf("������	������	������\n");
		printf("��E��	��C��	��C��\n");
		printf("������	������	������\n");
	}

	if (select == 0 && citynum == 1) { //Ȳ�� & �ù� 1��
		printf("������	������\n");
		printf("��?��	��?��\n");
		printf("������	������\n\n\n");
		printf("������	������\n");
		printf("��E��	��C��\n");
		printf("������	������\n");
	}

	if (select == 0 && citynum == 0) { //Ȳ�� & �ù� 0��
		printf("������\n");
		printf("��?��\n");
		printf("������\n\n\n");
		printf("������\n");
		printf("��E��\n");
		printf("������\n");
	}

	if (select == 1 && citynum == 4) { //�뿹 & �ù� 4��
		printf("������	������	������	������	������\n");
		printf("��?��	��?��	��?��	��?��	��?��\n");
		printf("������	������	������	������	������\n\n\n");
		printf("������	������	������	������	������\n");
		printf("��S��	��C��	��C��	��C��	��C��\n");
		printf("������	������	������	������	������\n");
	}

	if (select == 1 && citynum == 3) { //�뿹 & �ù� 3��
		printf("������	������	������	������\n");
		printf("��?��	��?��	��?��	��?��\n");
		printf("������	������	������	������\n\n\n");
		printf("������	������	������	������\n");
		printf("��S��	��C��	��C��	��C��\n");
		printf("������	������	������	������\n");
	}

	if (select == 1 && citynum == 2) { //�뿹 & �ù� 2��
		printf("������	������	������\n");
		printf("��?��	��?��	��?��\n");
		printf("������	������	������\n\n\n");
		printf("������	������	������\n");
		printf("��S��	��C��	��C��\n");
		printf("������	������	������\n");
	}

	if (select == 1 && citynum == 1) { //�뿹 & �ù� 1��
		printf("������	������\n");
		printf("��?��	��?��\n");
		printf("������	������\n\n\n");
		printf("������	������\n");
		printf("��S��	��C��\n");
		printf("������	������\n");
	}

	if (select == 1 && citynum == 0) { //�뿹 & �ù� 0��
		printf("������\n");
		printf("��?��\n");
		printf("������\n\n\n");
		printf("������\n");
		printf("��S��\n");
		printf("������\n");
	}

	line(50);
}

int get_user_card(int a, int b, int play, int comscore, int userscore) {
	int select;
	while (1) {
		system("cls");
		stat(play, comscore, userscore);
		show_table(a, b);
		printf("\n\n");
		printf("[ī�� ����]\n");
		line(25);
		printf("\n\n");


		if (a == 0) {
			printf("0. Ȳ�� (1��)\n\n");
			printf("1. �ù� (%d��)\n\n", b);
		}
		if (a == 1) {
			printf("0. �뿹 (1��)\n\n");
			printf("1. �ù� (%d��)\n\n", b);
		}

		line(25);
		printf("\n");
		printf("�Է� : ");
		scanf("%d", &select);
		printf("\n\n\n");

		if (select == 0) // Ȳ�� �Ǵ� �ù��̹Ƿ� ���� ���� 
			break;
		if (select == 1 && b != 0) // �ù�ī�� 1�� �̻��� ���̹Ƿ� ���� ���� 
			break;
		else { // �ù�ī�� 0���ε� �ù�ī�带 ������ �ϸ� error ǥ��
			error();
			continue;
		}
	}
	return select;
}

int get_computer_card(int citynum) { //���������� ���� �� �ùΰ��� ������� �ʰ� �ϱ� ���� if ��� 
	int ccard;
	if (citynum != 0)
		ccard = rand() % 2; // �뿹 or Ȳ�� : 0, �ù� : 1
	else
		ccard = 0;
	return ccard;
}

int compare_card(int EorS, int uchoice, int cchoice) {
	//��� ǥ���� ���� return ���� �ٸ��� ����. ¦���� ���� ��, Ȧ���� ���� ��, ���ºδ� ���� 
	if (EorS == 0) { //Ȳ�� 
		if (uchoice == 0 && cchoice == 0)//user : Ȳ�� vs �뿹 : com
			return 0;
		if (uchoice == 0 && cchoice == 1) //user : Ȳ�� vs �ù� : com
			return 1;
		if (uchoice == 1 && cchoice == 0) //user : �ù� vs �뿹 : com
			return 3;
		if (uchoice == 1 && cchoice == 1) //user : �ù� vs �ù� : com
			return -1;
	}

	if (EorS == 1) { //�뿹 
		if (uchoice == 0 && cchoice == 0) //user : �뿹 vs Ȳ�� : com
			return 5;
		if (uchoice == 0 && cchoice == 1) //user : �뿹 vs �ù� : com
			return 2;
		if (uchoice == 1 && cchoice == 0) //user : �ù� vs Ȳ�� : com
			return 4;
		if (uchoice == 1 && cchoice == 1) //user : �ù� vs �ù� : com
			return -1;
	}
}

void choice_result(int result) {

	printf("[���� ���]\n");
	line(25);
	printf("\n\n");
	switch (result) {
	case -1:
		printf("* ���� ī��	: �ù�\n\n");
		printf("* ��ǻ���� ī�� : �ù�\n\n");
		printf("* ��� : ���º�\n\n");
		break;
	case 0:
		printf("* ���� ī��	: Ȳ��\n\n");
		printf("* ��ǻ���� ī�� : �뿹\n\n");
		printf("* ��� : ���� �й�\n\n");
		break;
	case 1:
		printf("* ���� ī��	: Ȳ��\n\n");
		printf("* ��ǻ���� ī�� : �ù�\n\n");
		printf("* ��� : ���� �¸�\n\n");
		break;
	case 2:
		printf("* ���� ī��	: �뿹\n\n");
		printf("* ��ǻ���� ī�� : �ù�\n\n");
		printf("* ��� : ���� �й�\n\n");
		break;
	case 3:
		printf("* ���� ī��	: �ù�\n\n");
		printf("* ��ǻ���� ī�� : �뿹\n\n");
		printf("* ��� : ���� �¸�\n\n");
		break;
	case 4:
		printf("* ���� ī��	: �ù�\n\n");
		printf("* ��ǻ���� ī�� : Ȳ��\n\n");
		printf("* ��� : ���� �й�\n\n");
		break;
	case 5:
		printf("* ���� ī��	: �뿹\n\n");
		printf("* ��ǻ���� ī�� : Ȳ��\n\n");
		printf("* ��� : ���� �¸�\n\n");
		break;
	}
	line(25);
	printf("\n\n\n");
}

void stat(int play, int comscore, int userscore) {
	printf("[���� Ƚ�� : %d, ��ǻ�� : %d��, �� : %d��]", play, comscore, userscore);
}

void calc_score(int result, int *userscore, int *comscore) {
	switch (result) {
	case -1:
		*userscore = 0;
		*comscore = 0;
		break;
	case 0: //���� Ȳ���� ���� �� �й� 
		*userscore = -500;
		*comscore = 700;
		break;
	case 2: //���� �뿹�� ���� �� �й� 
	case 4:
		*userscore = -300;
		*comscore = 200;
		break;
	case 5: //���� �뿹�� ���� �� �¸�
		*userscore = 700;
		*comscore = -500;
		break;
	default: //���� Ȳ���� ���� �� �¸� 
		*userscore = 200;
		*comscore = -300;
	}

}

void print_score(int result, int* userpoint, int* compoint) {
	int userscore, comscore;
	calc_score(result, &userscore, &comscore);
	if (userscore == 0) // ���ºν� ��� ǥ�� ���� 
		return;
	else {
		*userpoint = *userpoint + userscore;
		*compoint = *compoint + comscore;
		printf("[���� ����]\n");
		line(25);
		printf("\n\n");
		printf("* ���� ����	: %d\n\n", *userpoint);
		printf("* ��ǻ���� ���� : %d\n\n", *compoint);
		line(25);
		printf("\n\n\n");
	}
}

void final_result(int userscore) {
	printf("[���� ���]\n");
	line(25);
	printf("\n\n");
	if (userscore >= 2000)
		printf("* ����� �¸�!\n\n");
	else //comscore�� 2000�� ����
		printf("* ����� �й�!\n\n");
	line(25);
	printf("\n\n");
	printf("EnterŰ�� �Է��ϼ���...");
	while (getchar() != '\n');
	getchar();
	system("cls");
}
