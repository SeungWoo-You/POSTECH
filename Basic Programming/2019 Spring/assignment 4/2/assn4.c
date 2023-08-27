#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WIDTH 60
#define MAX_LINES 1000
#pragma warning (disable : 6387)
#pragma warning (disable : 6011)

void divide_line(int num);
int txt_load(char name[31], char* line[MAX_LINES], int* linenum);
int word_wrap(FILE* txtfile, char* line[MAX_LINES]);
char* mystrrchr(char* str, char c);
int print_word(char* line[MAX_LINES], int count, int linenum);
char mode();
void deallocate(char* line[MAX_LINES], int linenum);
int alloc_error_check(char* line[MAX_LINES], int num);

int main() {
	char name[31] = "\0";
	char* line[MAX_LINES];
	int count = 0, temp = 0, linenum = 0, check = 0;

	while (1) {
		printf("* Text File Viewer *\n");
		if (txt_load(name, line, &linenum) == 0) {
			system("cls"); continue;
		}
		else {
			system("cls"); break;
		}
	}
	
	if (linenum == -1) {
		printf("error: some words exceed 60 length.\n");
		return -1;
	}	

	while (1) {
		temp = print_word(line, count, linenum);

		switch (mode()) {
		case 'f':
			if (temp * 10 < linenum)
				count = temp;
			else
				count = temp - 1;

			system("cls");
			break;
		case 'b':
			if (temp > 1)
				count = temp - 2;
			else
				count = temp - 1;

			system("cls");
			break;
		case 'o':
			if (txt_load(name, line, &linenum) == 0) {
				count = temp - 1;
				system("cls");

				if (linenum == -1) {
					printf("error: some words exceed 60 length.\n");
					return -1;
				}

				break;
			}

			count = 0;
			system("cls");
			break;
		case 'q':
			deallocate(line, linenum);
			return 0;
		default:
			system("cls");
			count = temp - 1;
			break;
		}
	}

	return 0;
}

void divide_line(int num) {
	int i;

	for (i = 0; i < num; i++)
		printf("=");

	return;
}

int txt_load(char name[31], char* line[MAX_LINES], int* linenum) {
	FILE* txtfile;
	char tempname[31] = "\0";
	errno_t err;
	static int check = 0;
	
	printf("filename: ");
	scanf_s("%s", tempname, sizeof(tempname));

	err = fopen_s(&txtfile, tempname, "r");
	if (err != 0) {
		printf("Unable to open %s\n", tempname);
		printf("Press Enter to continue...");
		while (getchar() != '\n');
		while (getchar() != '\n');
		return 0;
	}
	
	while (getchar() != '\n');

	strcpy_s(name, sizeof(char) * 31, tempname);

	if (check != 0)
		deallocate(line, *linenum);
	
	*linenum = word_wrap(txtfile, line);

	fclose(txtfile);

	return 1;
}

int word_wrap(FILE* txtfile, char* line[MAX_LINES]) {
	int i = 0, wcount = 0, lcount = 0;
	char* templine[MAX_LINES];
	char* tempword = NULL;

	line[i] = (char*)calloc(WIDTH + 2, sizeof(char));
	templine[i] = (char*)calloc(WIDTH + 2 - lcount, sizeof(char));

	if (alloc_error_check(line, i) || alloc_error_check(templine, i))
		return -2;

	for (i = 0; i < MAX_LINES; i++) {
		if (fgets(templine[i], WIDTH + 2 - lcount, txtfile) == NULL)
			break;
		
		if (templine[i][strlen(templine[i]) - sizeof(char)] == '\n') {
			wcount = 0;
			lcount = wcount;

			strcat_s(line[i], sizeof(char) * (WIDTH + 2), templine[i]);

			if (i + 1 == MAX_LINES)
				break;

			line[i + 1] = (char*)calloc(WIDTH + 2, sizeof(char));
			templine[i + 1] = (char*)calloc(WIDTH + 2 - lcount, sizeof(char));

			if (alloc_error_check(line, i) || alloc_error_check(templine, i))
				return -2;

			continue;
		}

		tempword = mystrrchr(templine[i], ' ');
		if (tempword != NULL) {
			wcount = strlen(tempword);
			templine[i][WIDTH - lcount - wcount] = '\0';

			strcat_s(line[i], sizeof(char) * (WIDTH + 2), templine[i]);
		}
		else
			return -1;

		lcount = wcount;

		if (i + 1 == MAX_LINES)
			break;

		line[i + 1] = (char*)calloc(WIDTH + 2, sizeof(char));
		templine[i + 1] = (char*)calloc(WIDTH + 2 - lcount, sizeof(char));

		if (alloc_error_check(line, i) || alloc_error_check(templine, i))
			return -2;

		strcpy_s(line[i + 1], sizeof(char) * (WIDTH + 2), tempword);
	}

	line[i] = "\0";

	deallocate(templine, i);

	return i;
}

char* mystrrchr(char* str, char c) {
	int i;

	i = strlen(str);
	i -= 1;
	for (i; i >= 0; i--) {
		if (str[i] == c)
			return str + i + 1;
	}

	return NULL;
}

int print_word(char* line[MAX_LINES], int count, int linenum) {
	int i;

	system("cls");
	for (i = count * 10; i < (count + 1) * 10; i++) {
		if (line[i][strlen(line[i]) - sizeof(char)] == '\n')
			printf("%s", line[i]);
		else
			printf("%s\n", line[i]);

		if (i == linenum)
			break;
	}
		
	divide_line(50);
	printf("[%d, %d]\n", count * 10 + 1, i);

	return ++count;
}

char mode() {
	char ch;

	printf("f, b, o, q> ");
	scanf_s("%c", &ch, sizeof(ch));

	if (getchar() != '\n') {
		while (getchar() != '\n');
		ch = 'e';
	}

	return ch;
}

void deallocate(char* line[MAX_LINES], int linenum) {
	int i;

	for (i = 0; i < linenum; i++)
		free(line[i]);

	return;
}

int alloc_error_check(char* line[MAX_LINES], int num) {
	if (line[num] == NULL)
		return 1;

	return 0;
}