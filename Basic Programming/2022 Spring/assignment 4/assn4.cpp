#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LINE 29  // '=' 개수
#define NAME_LEN 128  // 파일 이름 최대 길이
#define CHANNEL 3  // 채널 수
#define MIN_PIXEL 0  // 최소 픽셀 값
#define MAX_PIXEL 255  // 최대 픽셀 값

int Show_Menu();  // 메뉴 출력 함수
int*** create_image(int width, int height);  // 동적 할당 받기
void delete_image(int*** image, int height);  // 동적 할당 해제
int*** load_image(const char* file_name, int*** image, int* width, int* height);  // 이미지 불러오기
void save_image(const char* file_name, int*** image, int width, int height);  // 이미지 저장하기
void resize_image(int*** origin, int origin_width, int origin_height, int*** trans, int trans_width, int trans_height);  // 이미지 크기 재설정
int Linear_Interpolation(int** image, double x, double y);  // 선형 보간 계산
int Round(double num); // 반올림
int Calc_Value(int data);
double Backward_Mapping(int origin_length, int trans_length, int coordinate);  // 역방향 사상 계산
void line(int num, char letter);
void error();

int main() {
	int*** image = NULL, *** trans_image = NULL;  // trans는 resize에서 쓰는 임시 포인터
	int is_image = 0, width = 0, height = 0, trans_width = 0, trans_height = 0;  // is_image: 이미지 load 여부 확인
	char file_name[NAME_LEN] = "";  // 파일 이름 저장용 문자열

	while (1) {
		switch (Show_Menu()) {  // 1. 메뉴 출력. 반환 값을 이용해 switch case 사용
		case 1: // 2. 이미지 불러오기
			if (is_image == 1)  // 이미 이미지가 있으면 그 이미지를 삭제(동적 할당 해제)하고, 새롭게 이미지 저장
				delete_image(image, height);

			printf("Enter input filename: ");
			scanf("%s", &file_name);  // main에서 파일 이름 스캔
			image = load_image(file_name, image, &width, &height);

			if (image == NULL)  // 이미지 못 불러온 경우
				is_image = 0;

			else is_image = 1;  // 이미지 잘 불러온 경우

			break;

		case 2: // 3. 이미지 저장하기
			if (is_image == 0) {  // 3-1. 에러 처리: 이미지 없는 경우
				printf("Image is not loaded\n"); break;
			}

			printf("Enter output filename: ");
			scanf("%s", &file_name);  // main에서 파일 이름 스캔
			save_image(file_name, image, width, height);
			
			break;

		case 3: // 4. 이미지 크기 조절하기
			if (is_image == 0) {  // 4-1. 에러 처리: 이미지 없는 경우
				printf("Image is not loaded\n"); break;
			}

			printf("Enter output size: ");
			scanf("%d %d", &trans_width, &trans_height);

			if (trans_width <= 0 || trans_height <= 0) {  // 4-1. 에러 처리: 이상한 값 입력
				printf("Invalid size\n"); break;
			}

			trans_image = create_image(trans_width, trans_height);  // resize용 이미지 배열 생성
			
			resize_image(image, width, height, trans_image, trans_width, trans_height);

			delete_image(image, height);  // 기존 이미지 배열 삭제
			image = trans_image;  // 이미지 정보 다 바꿔주기
			width = trans_width;
			height = trans_height;

			break;

		case 0:  // quit
			if (is_image == 1)
				delete_image(image, height);
			printf("Quit program\n");
			
			return 0;

		default:
			error();	// 이외의 값 입력시 error 
		}

		printf("\n");
	}

	return 0;
}

int Show_Menu() {
	int select = -1;  // 메뉴 선택지에 0이 있기 때문에 -1로 설정

	line(LINE, '=');
	printf("│       IMAGE RESIZER       |\n");
	printf("│  1. Load      2. Save     |\n");
	printf("│  3. Resize    0. Quit     |\n");
	line(LINE, '=');

	printf("Enter number: ");
	scanf("%d", &select);

	return select;
}

int*** create_image(int width, int height) {
	int*** image = NULL;
	int i, j;

	image = (int***)malloc(sizeof(int**) * CHANNEL);	// pixel data 저장용 공간 할당받기. 필요한 만큼만 할당
	if (image == NULL) exit(0);  // 할당 실패시 강제 종료
	for (i = 0; i < CHANNEL; i++) {  // C -> H -> W 순서로 할당
		if (height >= 1) {  // C6385 & C6386 오류 해결용. 무시해도 되는 구문
			image[i] = (int**)malloc(sizeof(int*) * height);
			if (image[i] == NULL) exit(0);  // 할당 실패시 강제 종료
		}

		for (j = 0; j < height; j++)
			image[i][j] = (int*)malloc(sizeof(int) * width);
	}

	return image;
}

void delete_image(int*** image, int height) {
	int i, j;

	for (i = 0; i < CHANNEL; i++) {
		for (j = 0; j < height; j++)
			free(image[i][j]);
		free(image[i]);
	}

	free(image);
}

int*** load_image(const char* file_name, int*** image, int* width, int* height) {
	FILE* file = NULL;
	int check = 0;
	int i, j, k; // 각각 channel, height, width

	file = fopen(file_name, "r");

	if (file == NULL) {  // 2-1. 에러처리: 파일 읽기 실패
		printf("File not exists\n");

		return NULL;
	}

	fscanf(file, "%*c%*d %d %d %*d\n", width, height);  // width == col index, height == row index

	// 할당 받는 순서에 따라 i, j, k를 다르게 지정할 수 있고, 이에 따라 데이터 저장 구문도 바뀌어야 한다.
	image = create_image(*width, *height);

	// 지금의 할당 형태와 파일 구조를 고려하면 j(H), k(W), i(C) 순서로 for문을 구성해야 함 
	for (j = 0; j < *height; j++) {	 // 데이터 저장
		for (k = 0; k < *width; k++) {
			for (i = 0; i < CHANNEL; i++) {
				check = fscanf(file, "%d", &(image[i][j][k]));  // 오류 검사. assn3에서 했던 대로 적용
				if (check != 1 || image[i][j][k] < MIN_PIXEL || image[i][j][k] > MAX_PIXEL) {
					printf("Image courrupted\n");  // 2-1. 에러 처리: 이상한 파일
					fclose(file);

					delete_image(image, *height); // 이미지 파일 저장 실패 시 할당 해제하고 NULL 반환
					return NULL;
				}
			}
		}
	}

	if (fscanf(file, "%d", &check) == 1) {  // data 남아있는지 검사
		printf("Image courrupted\n");  // 2-1. 에러 처리: 이상한 파일
		fclose(file);

		return NULL;
	}

	printf("Read %s (%dx%d)\n", file_name, *width, *height);

	fclose(file);

	return image;

}

void save_image(const char* file_name, int*** image, int width, int height) {
	FILE* file = NULL;
	int i, j, k; // 각각 channel, height, width

	file = fopen(file_name, "w");	// 쓰기 모드로 입력 
	if (file == NULL) {	// 파일 생성에 실패한 경우 
		printf("\aError : Cannot create the image'%s'\n", file_name);

		return;
	}

	fprintf(file, "P%d %d %d %d\n", CHANNEL, width, height, MAX_PIXEL);	// 파일 구조 고려 
	for (j = 0; j < height; j++)// 데이터 저장. 파일 구조 고려해 j(Hi), k(Wi), i(Ci) 순서로 for문 구성
		for (k = 0; k < width; k++)
			for (i = 0; i < CHANNEL; i++)
				fprintf(file, "%d ", image[i][j][k]);

	fclose(file);

	return;
}

void resize_image(int*** origin, int origin_width, int origin_height, int*** trans, int trans_width, int trans_height) {
	double x = 0, y = 0;
	int i, j, k;

	for (j = 0; j < trans_height; j++) {
		y = Backward_Mapping(origin_height, trans_height, j);  // y 계산
		for (k = 0; k < trans_width; k++) {
			x = Backward_Mapping(origin_width, trans_width, k);  // x 계산
			for (i = 0; i < CHANNEL; i++) {
				trans[i][j][k] = Linear_Interpolation(origin[i], x, y);  // 각 channel에 대해 보간 계산
			}
		}
	}

	printf("Resized image (%dx%d -> %dx%d)\n", origin_width, origin_height, trans_width, trans_height);

	return;
}

int Linear_Interpolation(int** image, double x, double y) {
	int x_1 = (int)floor(x), x_2 = (int)ceil(x);
	int y_1 = (int)floor(y), y_2 = (int)ceil(y);
	double temp_f_1 = 0, temp_f_2 = 0, image_data = 0;

	if (x_1 == x_2) {  // 선형 보간 경우 검사
		if (y_1 == y_2)
			return image[y_1][x_1];

		image_data = (y_2 - y) / (y_2 - y_1) * image[y_1][x_1] + (y - y_1) / (y_2 - y_1) * image[y_2][x_1];
		return Calc_Value(Round(image_data));
	}

	else if (y_1 == y_2) {  // 선형 보간 경우 검사
		image_data = (x_2 - x) / (x_2 - x_1) * image[y_1][x_1] + (x - x_1) / (x_2 - x_1) * image[y_1][x_2];
		return Calc_Value(Round(image_data));
	}

	// 선형 보간 계산
	temp_f_1 = (x_2 - x) / (x_2 - x_1) * image[y_1][x_1] + (x - x_1) / (x_2 - x_1) * image[y_1][x_2];
	temp_f_2 = (x_2 - x) / (x_2 - x_1) * image[y_2][x_1] + (x - x_1) / (x_2 - x_1) * image[y_2][x_2];
	image_data = (y_2 - y) / (y_2 - y_1) * temp_f_1 + (y - y_1) / (y_2 - y_1) * temp_f_2;

	return Calc_Value(Round(image_data));
}

double Backward_Mapping(int origin_length, int trans_length, int coordinate) {  // 좌표 1개에 대해서 계산하는 함수
	return (origin_length - 1) / (double)(trans_length - 1) * coordinate;
}

int Round(double num) {
	return (int)floor(num + 0.5);
}


int Calc_Value(int data) {
	if (data <= MIN_PIXEL)
		return MIN_PIXEL;

	else if (data >= MAX_PIXEL)
		return MAX_PIXEL;
	
	return data;
}

void line(int num, char letter) {
	int i;

	for (i = 0; i < num; i++) {
		printf("%c", letter);	// 원하는 개수(num)만큼 letter 표시 
	}
	
	printf("\n");
}

void error() {
	printf("Invalid menu selected\n");
}