#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#define MAXROW 8
#define MAXCOLUMN 8
#define RED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
#define YEL SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
#define BLU SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
#define WHT SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

int ReadMap(char map[][MAXCOLUMN], int antCounts[][MAXCOLUMN], int* row, int
	* column, int* regionCount);
void line(int num);
void PrintMap(char map[][MAXCOLUMN], int row, int column);
void PrintAntCounts(char map[][MAXCOLUMN], int antCounts[][MAXCOLUMN], int
	row, int column);
void move_turn_human(char map[MAXROW][MAXCOLUMN], int antCounts[MAXROW][MAXCOLUMN], int row, int col);
void find_pos(int sel, int* count, int* rpos, int* cpos, int r, int c);
int move_sel_error(int start, int end);
void check_region(char map[MAXROW][MAXCOLUMN], int calc_ants[MAXROW][MAXCOLUMN], int sel, int rpos, int cpos, char type);
void move_turn_com(char map[MAXROW][MAXCOLUMN], int antCounts[MAXROW][MAXCOLUMN], int calc_ants[MAXROW][MAXCOLUMN], int row, int col);
void breeding_turn(char map[MAXROW][MAXCOLUMN], int antCounts[MAXROW][MAXCOLUMN], int row, int col);
int game_decision(char map[MAXROW][MAXCOLUMN], int row, int col);
void PrintResult(char map[][MAXCOLUMN], int antCounts[][MAXCOLUMN], int row, int column, int gameResult);

int main() {
	char map[MAXROW][MAXCOLUMN];
	int antCounts[MAXROW][MAXCOLUMN] = {0};
	int row = 0, col = 0, regionCount, i, j, result;

	srand(time(NULL));
	for (i = 0; i < MAXROW; i++) {
		for (j = 0; j < MAXCOLUMN; j++)
				map[i][j] = '0';
	}

	if (ReadMap(map, antCounts, &row, &col, &regionCount) == 0)
		return 0;
	while (1) {
		PrintMap(map, row, col);
		line(col * 3 + 2);
		PrintAntCounts(map, antCounts, row, col);
		printf("\n\n");

		move_turn_human(map, antCounts, row, col);
		line(col * 3 + 2);

		if (result = game_decision(map, row, col)) {
			PrintMap(map, row, col);
			line(col * 3 + 2);
			PrintAntCounts(map, antCounts, row, col);
			PrintResult(map, antCounts, row, col, result);
			return 0;
		}

		printf("ants fight each other!\n\n");
		PrintMap(map, row, col);
		line(col * 3 + 2);
		PrintAntCounts(map, antCounts, row, col);

		printf("\nants breeding!\n\n");
		breeding_turn(map, antCounts, row, col);
	}
}

int ReadMap(char map[][MAXCOLUMN], int antCounts[][MAXCOLUMN], int* row, int
	* column, int* regionCount) {
	FILE* flmap;
	int i, check = 0, Nrow = 0, Ncol = 0;

	fopen_s(&flmap, "map.txt", "r");
	if (flmap == NULL) {
		printf("\aFile opening error: The file 'map.txt' does not exist.\n");
		return 0;
	}

	fscanf_s(flmap, "%d%d", row, column);
	if (*row > MAXROW || *column > MAXCOLUMN) {
		printf("\aFile opening error: The map size in the file 'map.txt' is larger than max size(maximum: 8X8).\n");
		return 0;
	}

	fscanf_s(flmap, "%d", regionCount);
	for (i = 0; i < *regionCount; i++) {
		check = fscanf_s(flmap, "%d%d", &Nrow, &Ncol);
		if (check != 2) {
			printf("\aFile opening error: The file 'map.txt' does not match the number of Neutral Region.\n");
			return 0;
		}
		if (Nrow >= *row || Ncol >= *column) {
			printf("\aFile opening error: The file 'map.txt includes numbers out of range(range: %d to %d).\n", *row, *column);
			return 0;
		}
		if ((Nrow == 0 && Ncol == 0) || (Nrow == *row - 1 && Ncol == *column - 1)) {
			printf("\aFile opening error: The Neutral Region cannot be Basement at starting.\n");
			return 0;
		}

		map[Nrow][Ncol] = 'N';
	}

	check = 0;
	check = fscanf_s(flmap, "%d", &Nrow);
	if (check == 1) {
		printf("\aFile opening error : The file 'map.txt' does not match the number of eutral Region.\n");
		return 0;
	}

	map[0][0] = 'A';
	map[*row - 1][*column - 1] = 'B';
	antCounts[0][0] = 10;
	antCounts[*row - 1][*column - 1] = 10;

	fclose(flmap);

	return 1;
}

void line(int num) {
	int i;

	for (i = 0; i < num; i++)
		printf("-");
	printf("\n");
	return;
}

void PrintMap(char map[][MAXCOLUMN], int row, int column) {
	int i, j;

	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			if (map[i][j] == 'A') {
				RED printf("%3c", map[i][j]);
				continue;
			}
			if (map[i][j] == 'B') {
				BLU printf("%3c", map[i][j]);
				continue;
			}
			if (map[i][j] == 'N') {
				YEL printf("%3c", map[i][j]);
				continue;
			}
			else {
				WHT printf("%3c", map[i][j]);
			}
		}
		printf("\n");
	}
	WHT
}

void PrintAntCounts(char map[][MAXCOLUMN], int antCounts[][MAXCOLUMN], int
	row, int column) {
	int i, j;

	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			if (map[i][j] == 'A') {
				RED printf("%3d", antCounts[i][j]);
				continue;
			}
			if (map[i][j] == 'B') {
				BLU printf("%3d", antCounts[i][j]);
				continue;
			}
			if (map[i][j] == 'N') {
				YEL printf("%3d", antCounts[i][j]);
				continue;
			}
			else {
				WHT printf("%3d", antCounts[i][j]);
			}
		}
		printf("\n");
	}
	WHT
}

void move_turn_human(char map[MAXROW][MAXCOLUMN], int antCounts[MAXROW][MAXCOLUMN], int row, int col) {
	static int calc_ants[MAXROW][MAXCOLUMN] = {0};
	int i, j, count = 0, sel;
	int frpos = 0, fcpos = 0, grpos = 0, gcpos = 0;
	int ld = 0, rd = 0, ud = 0, dd = 0;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j] == 'B')
				calc_ants[i][j] = -antCounts[i][j];
			else
				calc_ants[i][j] = antCounts[i][j];
		}
	}

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j] == 'A') {
				printf("%d : (%d, %d)\n", ++count, i, j);
				continue;
			}
		}
	}

	printf("Where ants start? ");
	sel = move_sel_error(1, count);
	printf("\n");
	
	count = 0;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j] == 'A') {
				find_pos(sel, &count, &frpos, &fcpos, i, j);
				continue;
			}
		}
	}

	count = 1;
	for (i = 1; i < col; i++) {
		if (fcpos - i < 0)
			break;
		if (map[frpos][fcpos - i] == '0') {
			count++; continue;
		}
		else {
			ld = count; break;
		}
	}

	count = 1;
	for (i = 1; i < col; i++) {
		if (fcpos + i > col)
			break;
		if (map[frpos][fcpos + i] == '0') {
			count++; continue;
		}
		else {
			rd = count; break;
		}
	}

	for (i = 1; i < row; i++) {
		if (frpos - i < 0)
			break;
		for (j = 0; j < col; j++) {
			if (map[frpos - i][j] == '0')
				continue;
			else {
				ud = i; break;
			}
		}
		if (ud == i)
			break;
	}

	for (i = 1; i < row; i++) {
		if (frpos + i > row)
			break;
		for (j = 0; j < col; j++) {
			if (map[frpos + i][j] == '0') 
				continue;
			else {
				dd = i; break;
			}
		}
		if (dd == i)
			break;
	}
	
	count = 0;
	if(ld != 0)
		printf("%d : (%d, %d)\n", ++count, frpos, fcpos - ld);
	if(rd != 0)
		printf("%d : (%d, %d)\n", ++count, frpos, fcpos + rd);
	for (i = 1; i < col; i++) {
		if (ud != 0 && map[frpos - ud][i] != '0')
			printf("%d : (%d, %d)\n", ++count, frpos - ud, i);
		if (dd != 0 && map[frpos + dd][i] != '0')
			printf("%d : (%d, %d)\n", ++count, frpos + dd, i);
	}

	printf("Where ants go? ");
	sel = move_sel_error(1, count);
	printf("\n");

	count = 0;
	if (ld != 0)
		find_pos(sel, &count, &grpos, &gcpos, frpos, fcpos - ld);
	if (rd != 0)
		find_pos(sel, &count, &grpos, &gcpos, frpos, fcpos + rd);
	for (i = 1; i < col; i++) {
		if (ud != 0 && map[frpos - ud][i] != '0')
			find_pos(sel, &count, &grpos, &gcpos, frpos - ud, i);
		if (dd != 0 && map[frpos + dd][i] != '0')
			find_pos(sel, &count, &grpos, &gcpos, frpos + dd, i);
	}

	printf("How many ants go?(0 ~ %d) ", antCounts[frpos][fcpos] - 1);
	sel = move_sel_error(0, antCounts[frpos][fcpos] - 1);
	calc_ants[frpos][fcpos] -= sel;
	check_region(map, calc_ants, sel, grpos, gcpos, 'H');

	system("cls");
	printf("Human sends %d ants from (%d, %d) to (%d, %d)\n", sel, frpos, fcpos, grpos, gcpos);
	move_turn_com(map, antCounts, calc_ants, row, col);

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (calc_ants[i][j] < 0)
				antCounts[i][j] = -calc_ants[i][j];
			else
				antCounts[i][j] = calc_ants[i][j];
		}
	}

	return;
}

void find_pos(int sel, int* count, int* rpos, int* cpos, int r, int c) {
	++(*count);
	if (sel == *count) {
		*rpos = r; *cpos = c;
		return;
	}

	return;
}

int move_sel_error(int start, int end) {
	float select = 0;
	int sel = 0, errcount = 0;

	while (1) {
		scanf_s("%f", &select);
		sel = (int)select;
		while (getchar() != '\n')
			errcount++;
		if (sel < 1 || sel > end || sel - select != 0)
			errcount++;
		if (errcount) {
			printf("Please type an integer from %d to %d: ", start, end);
			errcount = 0; continue;
		}
		break;
	}

	return sel;
}

void check_region(char map[MAXROW][MAXCOLUMN], int calc_ants[MAXROW][MAXCOLUMN], int sel, int rpos, int cpos, char type) {
	if (type == 'H')
		calc_ants[rpos][cpos] += sel;
	else if (type == 'C')
		calc_ants[rpos][cpos] -= sel;

	if (calc_ants[rpos][cpos] > 0)
		map[rpos][cpos] = 'A';
	else if (calc_ants[rpos][cpos] < 0)
		map[rpos][cpos] = 'B';
	else if (calc_ants[rpos][cpos] == 0)
		map[rpos][cpos] = 'N';
}

void move_turn_com(char map[MAXROW][MAXCOLUMN], int antCounts[MAXROW][MAXCOLUMN],int calc_ants[MAXROW][MAXCOLUMN], int row, int col) {
	int i, j, count = 0, sel = 0;
	int frpos = 0, fcpos = 0, grpos = 0, gcpos = 0;
	int ld = 0, rd = 0, ud = 0, dd = 0;
	
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j] == 'B') {
				count++; continue;
			}
		}
	}
	
	sel = rand() % count + 1;
	count = 0;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j] == 'B') {
				find_pos(sel, &count, &frpos, &fcpos, i, j);
				continue;
			}
		}
	}
	
	count = 1;
	for (i = 1; i < col; i++) {
		if (fcpos - i < 0)
			break;
		if (map[frpos][fcpos - i] == '0') {
			count++; continue;
		}
		else {
			ld = count; break;
		}
	}

	count = 1;
	for (i = 1; i < col; i++) {
		if (fcpos + i > col)
			break;
		if (map[frpos][fcpos + i] == '0') {
			count++; continue;
		}
		else {
			rd = count; break;
		}
	}

	for (i = 1; i < row; i++) {
		if (frpos - i < 0)
			break;
		for (j = 0; j < col; j++) {
			if (map[frpos - i][j] == '0')
				continue;
			else {
				ud = i; break;
			}
		}
		if (ud == i)
			break;
	}

	for (i = 1; i < row; i++) {
		if (frpos + i > row)
			break;
		for (j = 0; j < col; j++) {
			if (map[frpos + i][j] == '0')
				continue;
			else {
				dd = i; break;
			}
		}
		if (dd == i)
			break;
	}

	count = 0;
	if (ld != 0)
		++count;
	if (rd != 0)
		++count;
	for (i = 1; i < col; i++) {
		if (ud != 0 && map[frpos - ud][i] != '0')
			++count;
		if (dd != 0 && map[frpos + dd][i] != '0')
			++count;
	}

	sel = rand() % count + 1;

	count = 0;
	if (ld != 0)
		find_pos(sel, &count, &grpos, &gcpos, frpos, fcpos - ld);
	if (rd != 0)
		find_pos(sel, &count, &grpos, &gcpos, frpos, fcpos + rd);
	for (i = 1; i < col; i++) {
		if (ud != 0 && map[frpos - ud][i] != '0')
			find_pos(sel, &count, &grpos, &gcpos, frpos - ud, i);
		if (dd != 0 && map[frpos + dd][i] != '0')
			find_pos(sel, &count, &grpos, &gcpos, frpos + dd, i);
	}
	
	sel = rand() % antCounts[frpos][fcpos];
	calc_ants[frpos][fcpos] += sel;
	check_region(map, calc_ants, sel, grpos, gcpos, 'C');

	printf("Computer sends %d ants from (%d, %d) to (%d, %d)\n", sel, frpos, fcpos, grpos, gcpos);

	return;
}

void breeding_turn(char map[MAXROW][MAXCOLUMN], int antCounts[MAXROW][MAXCOLUMN], int row, int col) {
	int i, j;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (map[i][j] == 'A') {
				if (i == 0 && j == 0)
					antCounts[i][j] += 2;
				else
					antCounts[i][j] += 1;
			}
			else if (map[i][j] == 'B') {
				if (i == row - 1 && j == col - 1)
					antCounts[i][j] += 2;
				else
					antCounts[i][j] += 1;
			}
		}
	}

	return;
}

int game_decision(char map[MAXROW][MAXCOLUMN], int row, int col) {
	if (map[0][0] == 'B' || map[0][0] == 'N') {
		if (map[row - 1][col - 1] == 'A' || map[row - 1][col - 1] == 'N') {
			system("cls");
			printf("draw.\n");
			return 1;
		}
		else {
			system("cls");
			printf("you Lose..\n");
			return 2;
		}
	}

	else {
		if (map[row - 1][col - 1] == 'A' || map[row - 1][col - 1] == 'N') {
			system("cls");
			printf("you win!!\n");
			return 3;
		}
	}

	return 0;
}

void PrintResult(char map[][MAXCOLUMN], int antCounts[][MAXCOLUMN], int row,
	int column, int gameResult) {
	FILE* flr;
	int i, j, Acount = 0, Bcount = 0, Aants = 0, Bants = 0;

	fopen_s(&flr, "replay.txt", "w");

	if (gameResult == 1)
		fprintf(flr, "D\n");
	if (gameResult == 2)
		fprintf(flr, "L\n");
	if (gameResult == 3)
		fprintf(flr, "W\n");
	for (i = 0; i < row; i++) {
		for (j = 0; j < column; j++) {
			if (map[i][j] == 'A') {
				Acount++; Aants += antCounts[i][j];
			}
			if (map[i][j] == 'B') {
				Bcount++; Bants += antCounts[i][j];
			}
		}
	}

	fprintf(flr, "%d %d\n", Acount, Aants);
	fprintf(flr, "%d %d", Bcount, Bants);

	fclose(flr);
}