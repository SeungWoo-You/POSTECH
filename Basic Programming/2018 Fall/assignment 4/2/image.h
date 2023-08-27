#ifndef IMAGE_H	// ������� �ߺ� ���� 
#define IMAGE_H

typedef struct {
    char name[100];    
    float*** data;	// .ppm�� pixel data�� �޴� ����
    int C, H, W;	// .ppm�� channel, height, width data ���� 
    int is_normalized;	// ����ȭ ���� �Ǵ�. ����ȭ�� �Ǿ��ִٸ� 1, �ƴ϶�� 0 
} CUBIC;
// ���α׷� ������ �ʿ��� �Լ� ���� 
CUBIC** ImageLoad(CUBIC** images, int* imgnum);		// �̹��� ���� ȣ�� 
CUBIC** ImageSave(CUBIC** images, int* imgnum);		// �̹��� ���� ���� 
CUBIC** ImageRemove(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum);		// �̹��� ���� 
CUBIC** FilterLoad(CUBIC** filters, int* filnum);	// ���� ���� ȣ�� 
CUBIC** FilterRemove(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum);	// ���� ���� 
CUBIC** Printimg(CUBIC** images, int* imgnum);		// �̹��� ��� ��� 
CUBIC** Printfil(CUBIC** filters, int* filnum);		// ���� ��� ��� 
CUBIC** Normalize(CUBIC** images, int* imgnum);		// �̹��� ����ȭ 
CUBIC** Denormalize(CUBIC** images, int* imgnum);	// �̹��� ������ȭ 
CUBIC** ImageConvol(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum);		// convolution ���� 
void Quit(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum);	// ���α׷� ���� 

void error();	// ���� ȣ�� 
char* Delppm(char *str);	// �̸����� .ppm ���� 
char* Plusppm(char *str, char *ppm);	// �̸��� .ppm �߰� 
char* Plusstr(char *str1, char *str2);	// �̸��� (str1_str2)�� ����� �Լ� 

#endif
