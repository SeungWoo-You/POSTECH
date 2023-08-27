#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LINE 29  // '=' ����
#define NAME_LEN 128  // ���� �̸� �ִ� ����
#define CHANNEL 3  // ä�� ��
#define MIN_PIXEL 0  // �ּ� �ȼ� ��
#define MAX_PIXEL 255  // �ִ� �ȼ� ��

int Show_Menu();  // �޴� ��� �Լ�
int*** create_image(int width, int height);  // ���� �Ҵ� �ޱ�
void delete_image(int*** image, int height);  // ���� �Ҵ� ����
int*** load_image(const char* file_name, int*** image, int* width, int* height);  // �̹��� �ҷ�����
void save_image(const char* file_name, int*** image, int width, int height);  // �̹��� �����ϱ�
void resize_image(int*** origin, int origin_width, int origin_height, int*** trans, int trans_width, int trans_height);  // �̹��� ũ�� �缳��
int Linear_Interpolation(int** image, double x, double y);  // ���� ���� ���
int Round(double num); // �ݿø�
int Calc_Value(int data);
double Backward_Mapping(int origin_length, int trans_length, int coordinate);  // ������ ��� ���
void line(int num, char letter);
void error();

int main() {
	int*** image = NULL, *** trans_image = NULL;  // trans�� resize���� ���� �ӽ� ������
	int is_image = 0, width = 0, height = 0, trans_width = 0, trans_height = 0;  // is_image: �̹��� load ���� Ȯ��
	char file_name[NAME_LEN] = "";  // ���� �̸� ����� ���ڿ�

	while (1) {
		switch (Show_Menu()) {  // 1. �޴� ���. ��ȯ ���� �̿��� switch case ���
		case 1: // 2. �̹��� �ҷ�����
			if (is_image == 1)  // �̹� �̹����� ������ �� �̹����� ����(���� �Ҵ� ����)�ϰ�, ���Ӱ� �̹��� ����
				delete_image(image, height);

			printf("Enter input filename: ");
			scanf("%s", &file_name);  // main���� ���� �̸� ��ĵ
			image = load_image(file_name, image, &width, &height);

			if (image == NULL)  // �̹��� �� �ҷ��� ���
				is_image = 0;

			else is_image = 1;  // �̹��� �� �ҷ��� ���

			break;

		case 2: // 3. �̹��� �����ϱ�
			if (is_image == 0) {  // 3-1. ���� ó��: �̹��� ���� ���
				printf("Image is not loaded\n"); break;
			}

			printf("Enter output filename: ");
			scanf("%s", &file_name);  // main���� ���� �̸� ��ĵ
			save_image(file_name, image, width, height);
			
			break;

		case 3: // 4. �̹��� ũ�� �����ϱ�
			if (is_image == 0) {  // 4-1. ���� ó��: �̹��� ���� ���
				printf("Image is not loaded\n"); break;
			}

			printf("Enter output size: ");
			scanf("%d %d", &trans_width, &trans_height);

			if (trans_width <= 0 || trans_height <= 0) {  // 4-1. ���� ó��: �̻��� �� �Է�
				printf("Invalid size\n"); break;
			}

			trans_image = create_image(trans_width, trans_height);  // resize�� �̹��� �迭 ����
			
			resize_image(image, width, height, trans_image, trans_width, trans_height);

			delete_image(image, height);  // ���� �̹��� �迭 ����
			image = trans_image;  // �̹��� ���� �� �ٲ��ֱ�
			width = trans_width;
			height = trans_height;

			break;

		case 0:  // quit
			if (is_image == 1)
				delete_image(image, height);
			printf("Quit program\n");
			
			return 0;

		default:
			error();	// �̿��� �� �Է½� error 
		}

		printf("\n");
	}

	return 0;
}

int Show_Menu() {
	int select = -1;  // �޴� �������� 0�� �ֱ� ������ -1�� ����

	line(LINE, '=');
	printf("��       IMAGE RESIZER       |\n");
	printf("��  1. Load      2. Save     |\n");
	printf("��  3. Resize    0. Quit     |\n");
	line(LINE, '=');

	printf("Enter number: ");
	scanf("%d", &select);

	return select;
}

int*** create_image(int width, int height) {
	int*** image = NULL;
	int i, j;

	image = (int***)malloc(sizeof(int**) * CHANNEL);	// pixel data ����� ���� �Ҵ�ޱ�. �ʿ��� ��ŭ�� �Ҵ�
	if (image == NULL) exit(0);  // �Ҵ� ���н� ���� ����
	for (i = 0; i < CHANNEL; i++) {  // C -> H -> W ������ �Ҵ�
		if (height >= 1) {  // C6385 & C6386 ���� �ذ��. �����ص� �Ǵ� ����
			image[i] = (int**)malloc(sizeof(int*) * height);
			if (image[i] == NULL) exit(0);  // �Ҵ� ���н� ���� ����
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
	int i, j, k; // ���� channel, height, width

	file = fopen(file_name, "r");

	if (file == NULL) {  // 2-1. ����ó��: ���� �б� ����
		printf("File not exists\n");

		return NULL;
	}

	fscanf(file, "%*c%*d %d %d %*d\n", width, height);  // width == col index, height == row index

	// �Ҵ� �޴� ������ ���� i, j, k�� �ٸ��� ������ �� �ְ�, �̿� ���� ������ ���� ������ �ٲ��� �Ѵ�.
	image = create_image(*width, *height);

	// ������ �Ҵ� ���¿� ���� ������ ����ϸ� j(H), k(W), i(C) ������ for���� �����ؾ� �� 
	for (j = 0; j < *height; j++) {	 // ������ ����
		for (k = 0; k < *width; k++) {
			for (i = 0; i < CHANNEL; i++) {
				check = fscanf(file, "%d", &(image[i][j][k]));  // ���� �˻�. assn3���� �ߴ� ��� ����
				if (check != 1 || image[i][j][k] < MIN_PIXEL || image[i][j][k] > MAX_PIXEL) {
					printf("Image courrupted\n");  // 2-1. ���� ó��: �̻��� ����
					fclose(file);

					delete_image(image, *height); // �̹��� ���� ���� ���� �� �Ҵ� �����ϰ� NULL ��ȯ
					return NULL;
				}
			}
		}
	}

	if (fscanf(file, "%d", &check) == 1) {  // data �����ִ��� �˻�
		printf("Image courrupted\n");  // 2-1. ���� ó��: �̻��� ����
		fclose(file);

		return NULL;
	}

	printf("Read %s (%dx%d)\n", file_name, *width, *height);

	fclose(file);

	return image;

}

void save_image(const char* file_name, int*** image, int width, int height) {
	FILE* file = NULL;
	int i, j, k; // ���� channel, height, width

	file = fopen(file_name, "w");	// ���� ���� �Է� 
	if (file == NULL) {	// ���� ������ ������ ��� 
		printf("\aError : Cannot create the image'%s'\n", file_name);

		return;
	}

	fprintf(file, "P%d %d %d %d\n", CHANNEL, width, height, MAX_PIXEL);	// ���� ���� ��� 
	for (j = 0; j < height; j++)// ������ ����. ���� ���� ����� j(Hi), k(Wi), i(Ci) ������ for�� ����
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
		y = Backward_Mapping(origin_height, trans_height, j);  // y ���
		for (k = 0; k < trans_width; k++) {
			x = Backward_Mapping(origin_width, trans_width, k);  // x ���
			for (i = 0; i < CHANNEL; i++) {
				trans[i][j][k] = Linear_Interpolation(origin[i], x, y);  // �� channel�� ���� ���� ���
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

	if (x_1 == x_2) {  // ���� ���� ��� �˻�
		if (y_1 == y_2)
			return image[y_1][x_1];

		image_data = (y_2 - y) / (y_2 - y_1) * image[y_1][x_1] + (y - y_1) / (y_2 - y_1) * image[y_2][x_1];
		return Calc_Value(Round(image_data));
	}

	else if (y_1 == y_2) {  // ���� ���� ��� �˻�
		image_data = (x_2 - x) / (x_2 - x_1) * image[y_1][x_1] + (x - x_1) / (x_2 - x_1) * image[y_1][x_2];
		return Calc_Value(Round(image_data));
	}

	// ���� ���� ���
	temp_f_1 = (x_2 - x) / (x_2 - x_1) * image[y_1][x_1] + (x - x_1) / (x_2 - x_1) * image[y_1][x_2];
	temp_f_2 = (x_2 - x) / (x_2 - x_1) * image[y_2][x_1] + (x - x_1) / (x_2 - x_1) * image[y_2][x_2];
	image_data = (y_2 - y) / (y_2 - y_1) * temp_f_1 + (y - y_1) / (y_2 - y_1) * temp_f_2;

	return Calc_Value(Round(image_data));
}

double Backward_Mapping(int origin_length, int trans_length, int coordinate) {  // ��ǥ 1���� ���ؼ� ����ϴ� �Լ�
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
		printf("%c", letter);	// ���ϴ� ����(num)��ŭ letter ǥ�� 
	}
	
	printf("\n");
}

void error() {
	printf("Invalid menu selected\n");
}