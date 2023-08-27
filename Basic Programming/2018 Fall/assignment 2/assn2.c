#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>

int show_menu();
void line(int);
void print_tutorial(); // 튜토리얼 표시 
int start_game(); // 게임 시작 
void end(); // 게임 종료 
void error(); // 잘못된 값 입력시 사용
int card_select(); // 황제 or 노예 패 선택 
void show_table(int, int); // 카드 모양 출력 
int get_user_card(int, int, int, int, int); // 유저 카드 선택 
int get_computer_card(int); // 컴퓨터 카드 랜덤 선택 
int compare_card(int, int, int); // 두 카드 비교 
void choice_result(int); // 선택 결과 표시 
void stat(int, int, int); // 통계(판 수, 점수) 표시 
void calc_score(int, int*, int*); // 점수 계산 
void print_score(int, int*, int*); // 점수 표시 
void final_result(int); // 최종 결과 표시

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
	printf("	1. 게임 설명	\n\n");
	printf("	2. 게임 시작	\n\n");
	printf("	3. 게임 종료	\n\n");
	line(34);
	printf("\n\n");
	printf("입력 : ");

	scanf("%d", &select);
	return select;
}

void line(int a) {
	int i;
	for (i = 0; i < a; i++) {
		printf("*"); //원하는 개수(a)만큼 별 표시 
	}
}

void print_tutorial() {
	system("cls");
	line(20);
	printf("	E - Card 게임 설명	");
	line(28);
	printf("\n\n");

	printf("본 게임은 도박묵시록 카이지의 E - Card 게임을 기반으로 한다.\n\n");
	printf("1. 두 플레이어는 황제패와 노예패 중 하나의 패를 각각 선택한다.\n");
	printf("   황제패 = 황제 카드 1장 + 시민 카드 4장\n");
	printf("   노예패 = 노예 카드 1장 + 시민 카드 4장\n\n");
	printf("2. 두 사용자는 매 턴마다 한 장의 카드를 안보이게 내밀고, 함께 뒤집는다.\n\n");
	printf("3. 아래의 카드 간 상성 관계에 따라 승패를 결정한다.\n");
	printf("   단, 두 플레이어가 시민 카드를 냈다면, 무승부로 처리한다.\n\n");
	printf("4. 내밀었던 카드는 소모되고 승패가 결정될 때까지 2로 돌아가 반복한다.\n\n");
	printf("상성관계 : 황제 > 시민 > 노예 > 황제\n\n");

	line(76);
	printf("\n");
	printf("메뉴로 돌아가려면 Enter키를 입력하세요...\n");
	while (getchar() != '\n'); //키보드 버퍼 값 초기화 
	getchar(); 
	// Enter 전에 여러 값을 입력한 경우 키보드 버퍼를 초기화 시켜 오류발생을 막기 위함 
}

int start_game() {
	int i, csel, getuc, getcc, choice;
	int turn = 1, userscore = 0, comscore = 0;
	char answer;

	while (userscore < 2000 && comscore < 2000) { //두 값이 2000미만일 때 작동하게 해 2000을 넘으면 메인으로 돌아가도록 함 
		system("cls");
		stat(turn, comscore, userscore);
		printf("\n\n\n");
		printf("[카드 패 선택]\n");
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

		for (i = 3; i > -1; i--) { // 무승부시 승부를 내기 위해 
			if (choice == -1) {
				printf("다음 턴 (Enter키를 입력하세요...)");
				while (getchar() != '\n');
				getchar();
				getuc = get_user_card(csel, i, turn, comscore, userscore);
				getcc = get_computer_card(i);
				choice = compare_card(csel, getuc, getcc); //위에서 결과를 표시했더라도 화면을 지웠기에 다시 표시 
				choice_result(choice);
			}
			else // 무승부 아닐 때 
				break;
		}
		print_score(choice, &userscore, &comscore);

		if (userscore < 2000 && comscore < 2000) { //유저 또는 컴퓨터의 점수가 2000 미만일 때만 표시되도록 함 
			printf("게임을 계속하시겠습니까? (y/n) : ");
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

	if (answer == 'n' || answer == 'N') // n을 응답했을 때 최종결과 표시 없이 main으로 돌아가기 위함 
		return 0;

	final_result(userscore);

	return 0;
}

void end() {
	printf("프로그램을 종료합니다...");
	exit(0); // 프로그램 종료
}

void error() {
	printf("올바른 메뉴를 선택하세요...");
	Sleep(1000);
	system("cls");
}

int card_select() {
	int select;
	line(30);
	printf("\n\n");
	printf("0. 황제패 (황제 1장, 시민 4장)\n\n");
	printf("1. 노예패 (노예 1장, 시민 4장)\n\n");
	line(30);
	printf("\n\n");
	printf("입력 : ");
	scanf("%d", &select);
	return select;
}

void show_table(int select, int citynum) {

	printf("\n\n");
	line(50);
	printf("\n");

	if (select == 0 && citynum == 4) { //황제 & 시민 4장 
		printf("┌─┐	┌─┐	┌─┐	┌─┐	┌─┐\n");
		printf("│?│	│?│	│?│	│?│	│?│\n");
		printf("└─┘	└─┘	└─┘	└─┘	└─┘\n\n\n");
		printf("┌─┐	┌─┐	┌─┐	┌─┐	┌─┐\n");
		printf("│E│	│C│	│C│	│C│	│C│\n");
		printf("└─┘	└─┘	└─┘	└─┘	└─┘\n");
	}

	if (select == 0 && citynum == 3) { //황제 & 시민 3장
		printf("┌─┐	┌─┐	┌─┐	┌─┐\n");
		printf("│?│	│?│	│?│	│?│\n");
		printf("└─┘	└─┘	└─┘	└─┘\n\n\n");
		printf("┌─┐	┌─┐	┌─┐	┌─┐\n");
		printf("│E│	│C│	│C│	│C│\n");
		printf("└─┘	└─┘	└─┘	└─┘\n");
	}

	if (select == 0 && citynum == 2) { //황제 & 시민 2장
		printf("┌─┐	┌─┐	┌─┐\n");
		printf("│?│	│?│	│?│\n");
		printf("└─┘	└─┘	└─┘\n\n\n");
		printf("┌─┐	┌─┐	┌─┐\n");
		printf("│E│	│C│	│C│\n");
		printf("└─┘	└─┘	└─┘\n");
	}

	if (select == 0 && citynum == 1) { //황제 & 시민 1장
		printf("┌─┐	┌─┐\n");
		printf("│?│	│?│\n");
		printf("└─┘	└─┘\n\n\n");
		printf("┌─┐	┌─┐\n");
		printf("│E│	│C│\n");
		printf("└─┘	└─┘\n");
	}

	if (select == 0 && citynum == 0) { //황제 & 시민 0장
		printf("┌─┐\n");
		printf("│?│\n");
		printf("└─┘\n\n\n");
		printf("┌─┐\n");
		printf("│E│\n");
		printf("└─┘\n");
	}

	if (select == 1 && citynum == 4) { //노예 & 시민 4장
		printf("┌─┐	┌─┐	┌─┐	┌─┐	┌─┐\n");
		printf("│?│	│?│	│?│	│?│	│?│\n");
		printf("└─┘	└─┘	└─┘	└─┘	└─┘\n\n\n");
		printf("┌─┐	┌─┐	┌─┐	┌─┐	┌─┐\n");
		printf("│S│	│C│	│C│	│C│	│C│\n");
		printf("└─┘	└─┘	└─┘	└─┘	└─┘\n");
	}

	if (select == 1 && citynum == 3) { //노예 & 시민 3장
		printf("┌─┐	┌─┐	┌─┐	┌─┐\n");
		printf("│?│	│?│	│?│	│?│\n");
		printf("└─┘	└─┘	└─┘	└─┘\n\n\n");
		printf("┌─┐	┌─┐	┌─┐	┌─┐\n");
		printf("│S│	│C│	│C│	│C│\n");
		printf("└─┘	└─┘	└─┘	└─┘\n");
	}

	if (select == 1 && citynum == 2) { //노예 & 시민 2장
		printf("┌─┐	┌─┐	┌─┐\n");
		printf("│?│	│?│	│?│\n");
		printf("└─┘	└─┘	└─┘\n\n\n");
		printf("┌─┐	┌─┐	┌─┐\n");
		printf("│S│	│C│	│C│\n");
		printf("└─┘	└─┘	└─┘\n");
	}

	if (select == 1 && citynum == 1) { //노예 & 시민 1장
		printf("┌─┐	┌─┐\n");
		printf("│?│	│?│\n");
		printf("└─┘	└─┘\n\n\n");
		printf("┌─┐	┌─┐\n");
		printf("│S│	│C│\n");
		printf("└─┘	└─┘\n");
	}

	if (select == 1 && citynum == 0) { //노예 & 시민 0장
		printf("┌─┐\n");
		printf("│?│\n");
		printf("└─┘\n\n\n");
		printf("┌─┐\n");
		printf("│S│\n");
		printf("└─┘\n");
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
		printf("[카드 선택]\n");
		line(25);
		printf("\n\n");


		if (a == 0) {
			printf("0. 황제 (1장)\n\n");
			printf("1. 시민 (%d장)\n\n", b);
		}
		if (a == 1) {
			printf("0. 노예 (1장)\n\n");
			printf("1. 시민 (%d장)\n\n", b);
		}

		line(25);
		printf("\n");
		printf("입력 : ");
		scanf("%d", &select);
		printf("\n\n\n");

		if (select == 0) // 황제 또는 시민이므로 정상 진행 
			break;
		if (select == 1 && b != 0) // 시민카드 1장 이상일 때이므로 정상 진행 
			break;
		else { // 시민카드 0장인데 시민카드를 내려고 하면 error 표출
			error();
			continue;
		}
	}
	return select;
}

int get_computer_card(int citynum) { //마지막까지 갔을 때 시민값을 출력하지 않게 하기 위해 if 사용 
	int ccard;
	if (citynum != 0)
		ccard = rand() % 2; // 노예 or 황제 : 0, 시민 : 1
	else
		ccard = 0;
	return ccard;
}

int compare_card(int EorS, int uchoice, int cchoice) {
	//결과 표출을 위해 return 값을 다르게 설정. 짝수는 유저 패, 홀수는 유저 승, 무승부는 음수 
	if (EorS == 0) { //황제 
		if (uchoice == 0 && cchoice == 0)//user : 황제 vs 노예 : com
			return 0;
		if (uchoice == 0 && cchoice == 1) //user : 황제 vs 시민 : com
			return 1;
		if (uchoice == 1 && cchoice == 0) //user : 시민 vs 노예 : com
			return 3;
		if (uchoice == 1 && cchoice == 1) //user : 시민 vs 시민 : com
			return -1;
	}

	if (EorS == 1) { //노예 
		if (uchoice == 0 && cchoice == 0) //user : 노예 vs 황제 : com
			return 5;
		if (uchoice == 0 && cchoice == 1) //user : 노예 vs 시민 : com
			return 2;
		if (uchoice == 1 && cchoice == 0) //user : 시민 vs 황제 : com
			return 4;
		if (uchoice == 1 && cchoice == 1) //user : 시민 vs 시민 : com
			return -1;
	}
}

void choice_result(int result) {

	printf("[선택 결과]\n");
	line(25);
	printf("\n\n");
	switch (result) {
	case -1:
		printf("* 나의 카드	: 시민\n\n");
		printf("* 컴퓨터의 카드 : 시민\n\n");
		printf("* 결과 : 무승부\n\n");
		break;
	case 0:
		printf("* 나의 카드	: 황제\n\n");
		printf("* 컴퓨터의 카드 : 노예\n\n");
		printf("* 결과 : 나의 패배\n\n");
		break;
	case 1:
		printf("* 나의 카드	: 황제\n\n");
		printf("* 컴퓨터의 카드 : 시민\n\n");
		printf("* 결과 : 나의 승리\n\n");
		break;
	case 2:
		printf("* 나의 카드	: 노예\n\n");
		printf("* 컴퓨터의 카드 : 시민\n\n");
		printf("* 결과 : 나의 패배\n\n");
		break;
	case 3:
		printf("* 나의 카드	: 시민\n\n");
		printf("* 컴퓨터의 카드 : 노예\n\n");
		printf("* 결과 : 나의 승리\n\n");
		break;
	case 4:
		printf("* 나의 카드	: 시민\n\n");
		printf("* 컴퓨터의 카드 : 황제\n\n");
		printf("* 결과 : 나의 패배\n\n");
		break;
	case 5:
		printf("* 나의 카드	: 노예\n\n");
		printf("* 컴퓨터의 카드 : 황제\n\n");
		printf("* 결과 : 나의 승리\n\n");
		break;
	}
	line(25);
	printf("\n\n\n");
}

void stat(int play, int comscore, int userscore) {
	printf("[게임 횟수 : %d, 컴퓨터 : %d점, 나 : %d점]", play, comscore, userscore);
}

void calc_score(int result, int *userscore, int *comscore) {
	switch (result) {
	case -1:
		*userscore = 0;
		*comscore = 0;
		break;
	case 0: //유저 황제패 선택 후 패배 
		*userscore = -500;
		*comscore = 700;
		break;
	case 2: //유저 노예패 선택 후 패배 
	case 4:
		*userscore = -300;
		*comscore = 200;
		break;
	case 5: //유저 노예패 선택 후 승리
		*userscore = 700;
		*comscore = -500;
		break;
	default: //유저 황제패 선택 후 승리 
		*userscore = 200;
		*comscore = -300;
	}

}

void print_score(int result, int* userpoint, int* compoint) {
	int userscore, comscore;
	calc_score(result, &userscore, &comscore);
	if (userscore == 0) // 무승부시 결과 표출 안함 
		return;
	else {
		*userpoint = *userpoint + userscore;
		*compoint = *compoint + comscore;
		printf("[현재 점수]\n");
		line(25);
		printf("\n\n");
		printf("* 나의 점수	: %d\n\n", *userpoint);
		printf("* 컴퓨터의 점수 : %d\n\n", *compoint);
		line(25);
		printf("\n\n\n");
	}
}

void final_result(int userscore) {
	printf("[최종 결과]\n");
	line(25);
	printf("\n\n");
	if (userscore >= 2000)
		printf("* 당신의 승리!\n\n");
	else //comscore가 2000을 넘음
		printf("* 당신의 패배!\n\n");
	line(25);
	printf("\n\n");
	printf("Enter키를 입력하세요...");
	while (getchar() != '\n');
	getchar();
	system("cls");
}
