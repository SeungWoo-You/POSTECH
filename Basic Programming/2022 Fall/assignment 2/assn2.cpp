#include <stdio.h>
#include <windows.h>
#include <math.h>

#define SIZE 75
#define ERR 0.00001
#define CHANNEL 3
#define PIXEL 255
#define H 0
#define S 1
#define V 2
#define R 0
#define G 1
#define B 2
#define MAX(a, b, c) ((a >= b && a >= c) ? a : (b >= a && b >= c) ? b : c)
#define MIN(a, b, c) ((a <= b && a <= c) ? a : (b <= a && b <= c) ? b : c)

int load_image(const char* filename, int image_rgb[][SIZE][SIZE], float image_hsv[][SIZE][SIZE], int* width, int* height);
void save_image(int image_rgb[][SIZE][SIZE], int width, int height);
void print_image(int image_rgb[][SIZE][SIZE], int width, int height);
void print_histogram(float image_hsv[][SIZE][SIZE], int width, int height);
void change_color(float image_hsv[][SIZE][SIZE], int width, int height, int source, int target);

void print_menu(const char* filename);
void RGB_to_HSV(int image_rgb[][SIZE][SIZE], float image_hsv[][SIZE][SIZE], int width, int height);
void HSV_to_RGB(int image_rgb[][SIZE][SIZE], float image_hsv[][SIZE][SIZE], int width, int height);
void HSV_to_RGB(int rgb[CHANNEL], float hsv[CHANNEL]);
void set_vt_mode();
void set_color_rgb(int r, int g, int b);
void reset_color();
int find_level(float hsv);

int main() {
	char filename[SIZE] = { 0 };
	int image_rgb[CHANNEL][SIZE][SIZE];
	float image_hsv[CHANNEL][SIZE][SIZE];
	int width = 0, height = 0;
	int select = 0, src = -1, target = -1;

	set_vt_mode();
	if (load_image(filename, image_rgb, image_hsv, &width, &height) == 0) {
		printf("File not found: %s", filename);
		return 0;
	}
		
	while (1) {
		print_menu(filename);
		while (1) {
			printf("Choose menu number >> ");
			scanf("%d", &select);
			while (getchar() != '\n');
			if (select < 1 || select > 5)
				printf("Wrong input!\n");
			else
				break;
		}

		switch (select) {
		case 1:
			print_histogram(image_hsv, width, height);
			break;
		case 2:
			print_histogram(image_hsv, width, height);
			while (1) {
				printf("Choose source number >> ");
				scanf("%d", &src);
				while (getchar() != '\n');
				if (src < 0 || src > 11)
					printf("Wrong input!\n");
				else
					break;
			}
			while (1) {
				printf("Choose target number >> ");
				scanf("%d", &target);
				while (getchar() != '\n');
				if (target < 0 || target > 11)
					printf("Wrong input!\n");
				else
					break;
			}
			change_color(image_hsv, width, height, src, target);
			HSV_to_RGB(image_rgb, image_hsv, width, height);
			break;
		case 3:
			print_image(image_rgb, width, height);
			break;
		case 4:
			save_image(image_rgb, width, height);
			break;
		case 5:
			return 0;
		}
	}
	return 0;
}

int load_image(const char* filename, int image_rgb[][SIZE][SIZE], float image_hsv[][SIZE][SIZE], int* width, int* height) {
	FILE* img = NULL;
	int i, j, k;
	

	printf("Enter input filename: ");
	scanf("%s", filename);
	while (getchar() != '\n');
	img = fopen(filename, "r");
	
	if (img == NULL) {
		return 0;
	}
	
	fscanf(img, "%*c%*d %d %d %*d\n", width, height);
	for (j = 0; j < *height; j++) {	// 데이터 저장
		for (k = 0; k < *width; k++) {
			for (i = 0; i < CHANNEL; i++)
				fscanf(img, "%d", &(image_rgb[i][j][k]));
		}
	}
	
	RGB_to_HSV(image_rgb, image_hsv, *width, *height);
	fclose(img);
	return 1;
}

void save_image(int image_rgb[][SIZE][SIZE], int width, int height) {
	FILE* img = NULL;
	int i, j, k;

	img = fopen("output.ppm", "w");
	if (img == NULL) {	// 파일 생성에 실패한 경우 
		printf("\aError : Cannot create output.ppm file.\n");
		exit(0);
	}

	fprintf(img, "P%d %d %d %d\n", CHANNEL, width, height, PIXEL);	// 파일 구조 고려
	for (j = 0; j < height; j++) {	// 데이터 저장. 파일 구조 고려해 j(H), k(W), i(C) 순서로 for문 구성
		for (k = 0; k < width; k++) {
			for (i = 0; i < CHANNEL; i++)
				fprintf(img, "%d ", image_rgb[i][j][k]);
		}
	}
	fclose(img);
}

void print_image(int image_rgb[][SIZE][SIZE], int width, int height) {
	int j, k;

	for (j = 0; j < height; j++) {
		for (k = 0; k < width; k++) {
			set_color_rgb(image_rgb[R][j][k], image_rgb[G][j][k], image_rgb[B][j][k]);
			printf(("■"));
		}
		printf("\n");
	}
	reset_color();
}

void print_histogram(float image_hsv[][SIZE][SIZE], int width, int height) {
	int histo_count[12] = { 0 }, rgb[CHANNEL] = { -1, -1, -1 };
	float hsv[CHANNEL] = { -1, 1, 1 };
	int level = -1, j, k;

	for (j = 0; j < height; j++) {
		for (k = 0; k < width; k++) {
			if (fabsf(image_hsv[S][j][k] - 0) < ERR)
				continue;
			level = (int)(image_hsv[H][j][k] / 30);
			//level = find_level(image_hsv[H][j][k]);
			histo_count[level] += 1;
			
		}
	}

	for (level = 0; level < 12; level++) {
		printf("[%2d]", level);
		hsv[H] = (float)level * 30;
		HSV_to_RGB(rgb, hsv);
		set_color_rgb(rgb[R], rgb[G], rgb[B]);
		for (j = 0; j < histo_count[level] / 10; j++)
			printf(("■"));
		reset_color();
		printf("%d\n", histo_count[level]);
	}
	printf("\n");
	reset_color();
}

void change_color(float image_hsv[][SIZE][SIZE], int width, int height, int source, int target) {
	int j, k;

	for (j = 0; j < height; j++) {
		for (k = 0; k < width; k++) {
			image_hsv[H][j][k] += (target - source) * 30;
			if (image_hsv[H][j][k] < 0)
				image_hsv[H][j][k] += 360;
			else if (image_hsv[H][j][k] >= 360)
				image_hsv[H][j][k] -= 360;
		}
	}
}

void print_menu(const char* filename) {
	printf("=======================\n");
	printf("  IMAGE COLOR CHANGER  \n");
	printf("=======================\n");
	printf("  1. Image Histogram\n");
	printf("  2. Change Color\n");
	printf("  3. Print Image\n");
	printf("  4. Save Image\n");
	printf("  5. Exit\n");
	printf("Loaded file: %s\n\n", filename);
}

void RGB_to_HSV(int image_rgb[][SIZE][SIZE], float image_hsv[][SIZE][SIZE], int width, int height) {
	float R_ = 0, G_ = 0, B_ = 0;
	float C_max = 0, C_min = 0, delta = 0;
	int j, k;
	
	for (j = 0; j < height; j++) {
		for (k = 0; k < width; k++) {
			R_ = (float)image_rgb[R][j][k] / PIXEL;
			G_ = (float)image_rgb[G][j][k] / PIXEL;
			B_ = (float)image_rgb[B][j][k] / PIXEL;
			C_max = MAX(R_, G_, B_);
			C_min = MIN(R_, G_, B_);
			delta = C_max - C_min;

			if (delta == 0) // calc H
				image_hsv[H][j][k] = 0;
			else if (C_max == R_)
				image_hsv[H][j][k] = 60 * ((G_ - B_) / delta);
			else if (C_max == G_)
				image_hsv[H][j][k] = 60 * ((B_ - R_) / delta + 2);
			else if (C_max == B_)
				image_hsv[H][j][k] = 60 * ((R_ - G_) / delta + 4);
			
			if (C_max == 0) // calc S
				image_hsv[S][j][k] = 0;
			else
				image_hsv[S][j][k] = delta / C_max;

			image_hsv[V][j][k] = C_max; // calc V
			
			if (image_hsv[H][j][k] < 0)
				image_hsv[H][j][k] += 360;
		}
	}
}

void HSV_to_RGB(int image_rgb[][SIZE][SIZE], float image_hsv[][SIZE][SIZE], int width, int height) {
	float R_ = 0, G_ = 0, B_ = 0;
	float C = 0, X = 0, m = 0;
	int j, k;

	for (j = 0; j < height; j++) {
		for (k = 0; k < width; k++) {
			// calc C, X, m
			C = image_hsv[V][j][k] * image_hsv[S][j][k];
			X = C * (1 - (float)fabs(fmod(image_hsv[H][j][k] / 60, 2) - 1));
			m = image_hsv[V][j][k] - C;

			// calc R_, G_, B_
			if (0 <= image_hsv[H][j][k] && image_hsv[H][j][k] < 60) {
				R_ = C;
				G_ = X;
				B_ = 0;
			}
			else if (60 <= image_hsv[H][j][k] && image_hsv[H][j][k] < 120) {
				R_ = X;
				G_ = C;
				B_ = 0;
			}
			else if (120 <= image_hsv[H][j][k] && image_hsv[H][j][k] < 180) {
				R_ = 0;
				G_ = C;
				B_ = X;
			}
			else if (180 <= image_hsv[H][j][k] && image_hsv[H][j][k] < 240) {
				R_ = 0;
				G_ = X;
				B_ = C;
			}
			else if (240 <= image_hsv[H][j][k] && image_hsv[H][j][k] < 300) {
				R_ = X;
				G_ = 0;
				B_ = C;
			}
			else if (300 <= image_hsv[H][j][k] && image_hsv[H][j][k] < 360) {
				R_ = C;
				G_ = 0;
				B_ = X;
			}

			// calc R, G, B
			image_rgb[R][j][k] = (int)((R_ + m) * 255);
			image_rgb[G][j][k] = (int)((G_ + m) * 255);
			image_rgb[B][j][k] = (int)((B_ + m) * 255);
		}
	}
}

void HSV_to_RGB(int rgb[CHANNEL], float hsv[CHANNEL]) {
	float R_ = 0, G_ = 0, B_ = 0;
	float C = 0, X = 0, m = 0;

	// calc C, X, m
	C = hsv[V] * hsv[S];
	X = C * (1 - (float)fabs(fmod(hsv[H] / 60, 2) - 1));
	m = hsv[V] - C;

	// calc R_, G_, B_
	if (0 <= hsv[H] && hsv[H] < 60) {
		R_ = C;
		G_ = X;
		B_ = 0;
	}
	else if (60 <= hsv[H] && hsv[H] < 120) {
		R_ = X;
		G_ = C;
		B_ = 0;
	}
	else if (120 <= hsv[H] && hsv[H] < 180) {
		R_ = 0;
		G_ = C;
		B_ = X;
	}
	else if (180 <= hsv[H] && hsv[H] < 240) {
		R_ = 0;
		G_ = X;
		B_ = C;
	}
	else if (240 <= hsv[H] && hsv[H] < 300) {
		R_ = X;
		G_ = 0;
		B_ = C;
	}
	else if (300 <= hsv[H] && hsv[H] < 360) {
		R_ = C;
		G_ = 0;
		B_ = X;
	}

	rgb[R] = (int)((R_ + m) * 255);
	rgb[G] = (int)((G_ + m) * 255);
	rgb[B] = (int)((B_ + m) * 255);
}

void set_vt_mode() {
	DWORD l_mode;
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &l_mode);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), l_mode | 0x0004 | 0x0008);
}

void set_color_rgb(int r, int g, int b) {
	printf("\033[38;2;%d;%d;%dm", r, g, b);
}

void reset_color() {
	printf("\033[0m");
}

int find_level(float hsv) {
	int level;
	float lev_err = hsv / 30;

	for (level = 0; level < 12; level++) {
		if (fabsf(lev_err - level) < ERR)
			return level;
		else if (lev_err <= level + 1 - ERR) {
			return level;
		}
		else
			continue;
	}
	printf("ERROR: Cannot find the proper level.\n");
	exit(0);
}