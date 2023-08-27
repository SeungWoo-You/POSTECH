#ifndef IMAGE_H	// 헤더파일 중복 방지 
#define IMAGE_H

typedef struct {
    char name[100];    
    float*** data;	// .ppm의 pixel data를 받는 공간
    int C, H, W;	// .ppm의 channel, height, width data 저장 
    int is_normalized;	// 정규화 유무 판단. 정규화가 되어있다면 1, 아니라면 0 
} CUBIC;
// 프로그램 구성에 필요한 함수 정의 
CUBIC** ImageLoad(CUBIC** images, int* imgnum);		// 이미지 파일 호출 
CUBIC** ImageSave(CUBIC** images, int* imgnum);		// 이미지 파일 저장 
CUBIC** ImageRemove(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum);		// 이미지 제거 
CUBIC** FilterLoad(CUBIC** filters, int* filnum);	// 필터 파일 호출 
CUBIC** FilterRemove(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum);	// 필터 제거 
CUBIC** Printimg(CUBIC** images, int* imgnum);		// 이미지 목록 출력 
CUBIC** Printfil(CUBIC** filters, int* filnum);		// 필터 목록 출력 
CUBIC** Normalize(CUBIC** images, int* imgnum);		// 이미지 정규화 
CUBIC** Denormalize(CUBIC** images, int* imgnum);	// 이미지 역정규화 
CUBIC** ImageConvol(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum);		// convolution 진행 
void Quit(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum);	// 프로그램 종료 

void error();	// 에러 호출 
char* Delppm(char *str);	// 이름에서 .ppm 제거 
char* Plusppm(char *str, char *ppm);	// 이름에 .ppm 추가 
char* Plusstr(char *str1, char *str2);	// 이름을 (str1_str2)로 만드는 함수 

#endif
