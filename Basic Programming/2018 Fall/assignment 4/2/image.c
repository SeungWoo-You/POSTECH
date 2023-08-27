#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

CUBIC** ImageLoad(CUBIC** images, int* imgnum) {
	FILE* imgfile;
	int i, j, k, Inum;	// i는 C, j는 H, k는 W에 대응 
	int C, H, W;	// channal, height, width
	
	Inum = *imgnum; 
	
	if(Inum  == 0)
	images = (CUBIC**)malloc(sizeof(CUBIC*));	// 목록에 이미지가 없으면 처음부터 할당 
	else
	images = (CUBIC**)realloc(images, sizeof(CUBIC*) * (Inum + 1));	// 목록에 이미지가 있으면 추가공간 할당받기 
	
	images[Inum] = (CUBIC*)malloc(sizeof(CUBIC));
	printf("Enter image filename : ");
	scanf("%s", images[Inum] -> name);	// 이름 저장 
	
	imgfile = fopen(images[Inum] -> name, "r");
	if(imgfile == NULL) {	// 해당 파일 없을 때 
		printf("\aError : The file '%s' does not exist.\n", images[Inum] -> name);
		free(images[Inum]);	// 저장되는 정보가 없으므로 할당받은 공간 해제 
		return images;
	}

	fscanf(imgfile, "%*c%d %d %d %*d\n", &images[Inum] -> C, &images[Inum] -> W, &images[Inum]-> H);	// P3 600 370 255에서 P와 255는 필요없는 정보이므로 버림 
	C = images[Inum] -> C;
	H = images[Inum] -> H;
	W = images[Inum] -> W;
	
	images[Inum] -> data = (float***)malloc(sizeof(float**) * C);	// pixel data 저장용 공간 할당받기. 필요한 만큼만 할당 
	for(i = 0; i < C; i++) {
		images[Inum] -> data[i] = (float**)malloc(sizeof(float*) * H);
		for(j = 0; j < H; j++) {
			images[Inum] -> data[i][j] = (float*)malloc(sizeof(float) * W);
		}
	}
	// 파일 구조를 고려하면 j(H), k(W), i(C) 순서로 for문을 구성해야 함 
	for(j = 0; j < H; j++) {	// 데이터 저장
		for(k = 0; k < W; k++) {
			for(i = 0; i < C; i++) {
				fscanf(imgfile, "%f", &(images[Inum] -> data[i][j][k]));
			}
		}
	}
	
	images[Inum] -> is_normalized = 0;	// 처음에는 정규화가 되어있지 않으므로 0 
	
	Delppm(images[Inum] -> name);	// 이름에서 .ppm 제거
	
	Inum += 1;	// 이미지 수 +1
	*imgnum = Inum;
	
	fclose(imgfile);
	
	return images;
}

CUBIC** ImageSave(CUBIC** images, int* imgnum) {
	FILE* imgfile;
	int i, j, k, select, Inum;	// i는 Ci, j는 Hi, k는 Wi에 대응 
	int Ci, Hi, Wi;	// channel, height, width
	char name[100];	// 이름 저장용 
	
	Inum = *imgnum;
	if(Inum == 0) {	// 이미지 없을 때 
		printf("\aError : There are no images to save in the list.\n");
		return images;
	}
	Printimg(images, imgnum);	// 이미지 목록 출력 
	
	printf("Enter the number of image to save : ");
		scanf("%d", &select);
		if(select >= Inum || select < 0) {	// 목록에 없는 값 입력시 오류 
			error(); return images;
	}
	
	Ci = images[select] -> C;
	Hi = images[select] -> H;
	Wi = images[select] -> W;
	
	printf("Enter filename for save image : ");
	scanf("%s", name);
	
	imgfile = fopen(name, "w");	// 쓰기 모드로 입력 
	if(imgfile == NULL) {	// 파일 생성에 실패한 경우 
		printf("\aError : Cannot create the image'%s'\n", name);
		return images;
	}
	
	fprintf(imgfile, "%c%d\n", 'P', Ci);	// 파일 구조 고려 
	fprintf(imgfile, "%d %d\n" , Wi, Hi);
	fprintf(imgfile, "%d\n", 255);
	
	for(j = 0; j < Hi; j++) {	// 데이터 저장. 파일 구조 고려해 j(Hi), k(Wi), i(Ci) 순서로 for문 구성
		for(k = 0; k < Wi; k++) {
			for(i = 0; i < Ci; i++) {
				if((int)(images[select] -> data[i][j][k]) < 0) 
				fprintf(imgfile, "%d ", 0);
				else if((int)(images[select] -> data[i][j][k]) > 255)
				fprintf(imgfile, "%d ", 255);
				else
				fprintf(imgfile, "%d ",(int)(images[select] -> data[i][j][k]));	// 정수로 저장 
			}
		}
	}
	
	fclose(imgfile);
	
	return images;
}

CUBIC** ImageRemove(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum) {
	FILE* imgfile;
	int q, i, j, k, select, Inum; // i는 C, j는 H, k는 W에 대응 
	int C, H, W;
	char name[100], ppm[5] = ".ppm";	// name은 원본 데이터 변경을 막기 위해, ppm은 ".ppm" 추가를 위해 
	
	Inum = *imgnum;
	if(Inum == 0) {	// 이미지 없을 때 
		printf("\aError : There are no images to erase in the list.\n");
		return images;
	}
	
	Printimg(images, imgnum);	// 이미지 목록 출력 
	printf("Enter the number of image to remove : ");
	scanf("%d", &select);
	
	if(select >= Inum || select < 0) {	// 목록에 없는 값 입력시 오류 
		error(); return images;
	}
	
	for(q = select; q < Inum; q++) {	// 선택한 이미지를 포함해 이보다 목록의 아래쪽에 있는 이미지들의 pixel data 할당 해제 
		C = images[q] -> C;				// 필요한 만큼의 공간만 할당받았으므로 이미지공간을 압축하기 위해서는 pixel data를 할당 해제 한 뒤 새로 필요한 만큼 할당받아야 함 
		H = images[q] -> H;				// 이렇게 안하면 할당받지 않은 공간을 참조하게 될 수도 있음 
		for(i = 0; i < C; i++) {
			for(j = 0; j < H; j++) {
				free(images[q] -> data[i][j]);
			}
			free(images[q] -> data[i]);
		}
		free(images[q] -> data);
	}
	 
	for(q = select; q < Inum - 1; q++) {	// pixel data를 제외한 나머지는 그대로 복사해도 괜찮음. 대신 마지막 공간을 할당해제 하기 전에 복사해야 함 
		strcpy(images[q] -> name, images[q + 1] -> name);	// 지울 이미지를 기준으로 한칸 뒤에 있는 이미지의 정보를 복사함
		images[q] -> C = images[q + 1] -> C;
		images[q] -> H = images[q + 1] -> H;
		images[q] -> W = images[q + 1] -> W;
		images[q] -> is_normalized = images[q + 1] -> is_normalized;
	}
	
	free(images[Inum - 1]);	// 가장 마지막에 저장된 이미지의 정보는 바로 앞칸으로 옮겼으므로 할당 해제하면 지울 이미지만 정보가 없어져있음 
	images = (CUBIC**)realloc(images, sizeof(CUBIC*) * (Inum - 1));	// 공간 압축 
	// 현재 포인터 배열의 공간이 압축된 상태. 하지만 선택한 이미지보다 아래쪽에 있던 이미지들의 pixel data가 사라진 상태이므로 이를 저장해주어야 함 
	for(q = select; q < Inum - 1; q++) {	// pixel data 저장하기 
		strcpy(name, images[q] -> name);	// 원본 데이터의 변경을 막기 위해 name으로 이름을 복사해 name을 편집 
		Plusppm(name, ppm);	// .ppm 추가 
		imgfile = fopen(name, "r");
		if(imgfile == NULL) {	// 파일 없을 때 
			printf("\aError : Failed to delete the image.\n");	// 파일이 없으면 사라진 pixel data의 정보를 저장할 방법이 없으므로 프로그램 강제 종료
			printf("Forcibly terminates the program due to an error.\n");
			Quit(images, filters, imgnum, filnum);
		} 
		
		fscanf(imgfile, "%*c%*d %*d %*d %*d\n");	// 필요한 것은 pixel data이므로 나머지는 버림 
		
		C = images[q] -> C;
		H = images[q] -> H;
		W = images[q] -> W;
		
		images[q] -> data = (float***)malloc(sizeof(float**) * C);	// pixel data 저장용 공간 할당받기 
		for(i = 0; i < C; i++) {
			images[q] -> data[i] = (float**)malloc(sizeof(float*) * H);
			for(j = 0; j < H; j++) {
				images[q] -> data[i][j] = (float*)malloc(sizeof(float) * W);
			}
		} 
		// 파일 구조를 고려하면 j, k, i 순서로 for문을 구성해야 함. 
		for(j = 0; j < H; j++) {	// 데이터 저장
			for(k = 0; k < W; k++) {
				for(i = 0; i < C; i++) {
					fscanf(imgfile, "%f", &(images[q] -> data[i][j][k]));
				}
			}
		}
	}
	
	Inum -= 1;	// 이미지 수 -1 
	*imgnum = Inum;
	
	return images;
}

CUBIC** FilterLoad(CUBIC** filters, int* filnum) {	// ImageLoad 함수와 비슷한 형식으로 구성 
	FILE* filfile;
	int i, j, k, Fnum;	// i는 C, j는 H, k는 W에 대응 
	int C, H, W;
	
	Fnum = *filnum;
	if(Fnum  == 0)
	filters = (CUBIC**)malloc(sizeof(CUBIC*));	// 목록에 필터가 없으면 처음부터 할당 
	else
	filters = (CUBIC**)realloc(filters, (Fnum + 1) * sizeof(CUBIC*));	// 목록에 필터가 있으면 추가공간 할당받기 
	
	printf("Enter filter filename : ");
	filters[Fnum] = (CUBIC*)malloc(sizeof(CUBIC));
	scanf("%s", filters[Fnum] -> name);	// 이름 저장 
	
	filfile = fopen(filters[Fnum] -> name, "r");
	if(filfile == NULL) {	// 해당 파일 없을 때 
		printf("\aError : The file '%s' does not exist.\n", filters[Fnum] -> name);
		free(filters[Fnum]);	// 저장되는 정보가 없으므로 할당받은 공간 해제 
		return filters;
	}
	
	fscanf(filfile, "%*c%d %d %d %*d\n", &filters[Fnum] -> C, &filters[Fnum] -> W, &filters[Fnum]-> H);	// P1 3 2 255에서 P와 255는 필요없는 정보이므로 버림 
	C = filters[Fnum] -> C;
	H = filters[Fnum] -> H;
	W = filters[Fnum] -> W;
		
	filters[Fnum] -> data = (float***)malloc(sizeof(float**) * C);	// pixel data 저장용 공간 할당받기. 필요한 만큼만 할당 
	for(i = 0; i < C; i++) {
		filters[Fnum] -> data[i] = (float**)malloc(sizeof(float*) * H);
		for(j = 0; j < H; j++) {
			filters[Fnum] -> data[i][j] = (float*)malloc(sizeof(float) * W);
		}
	}
	// 파일 구조를 고려하면 j(H), k(W), i(C) 순서로 for문을 구성해야 함 
	for(j = 0; j < H; j++) {	// 데이터 저장
		for(k = 0; k < W; k++) {
			for(i = 0; i < C; i++) {
				fscanf(filfile, "%f", &(filters[Fnum] -> data[i][j][k]));
			}
		}
	}
	
	Delppm(filters[Fnum] -> name);	// 이름에서 .ppm 제거
	
	Fnum += 1;	// 필터수 +1 
	*filnum = Fnum;
	
	fclose(filfile);
	
	return filters;
}

CUBIC** FilterRemove(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum) {
	FILE* filfile;
	int q, i, j, k, select, Fnum;	// i는 C, j는 H, k는 W에 대응
	int  C, H, W; 
	char name[100], ppm[5] = ".ppm";	// name은 원본 데이터 변경을 막기 위해, ppm은 ".ppm" 추가를 위해 
	
	Fnum = *filnum;
	if(Fnum == 0) {	// 필터 없을 때 
		printf("\aError : There are no filters to erase in the list.\n");
		return filters;
	}
	
	Printfil(filters, filnum);	// 필터 목록 출력 
	printf("Enter the number of filters to remove : ");
	scanf("%d", &select);
	
	if(select >= Fnum || select < 0) {	// 목록에 없는 값 입력시 오류 
		error(); return filters;
	}
	
	for(q = select; q < Fnum; q++) {	// 선택한 필터를 포함해 이보다 목록의 아래쪽에 있는 필터들의 pixel data 할당 해제
		C = filters[q] -> C;			// 필요한 만큼의 공간만 할당받았으므로 필터공간을 압축하기 위해서는 pixel data를 할당 해제 한 뒤 새로 필요한 만큼 할당받아야 함 
		H = filters[q] -> H; 			// 이렇게 안하면 할당받지 않은 공간을 참조하게 될 수도 있음 
		for(i = 0; i < C; i++) {
			for(j = 0; j < H; j++) {
				free(filters[q] -> data[i][j]);
			}
			free(filters[q] -> data[i]);
		}
		free(filters[q] -> data);
	}
	
	for(q = select; q < Fnum - 1; q++) {	// pixel data를 제외한 나머지는 그대로 복사해도 괜찮음. 대신 마지막 공간을 할당해제 하기 전에 복사해야 함 
		strcpy(filters[q] -> name, filters[q + 1] -> name);	// 지울 필터를 기준으로 한칸 뒤에 있는 필터의 정보를 복사함
		filters[q] -> C = filters[q + 1] -> C;
		filters[q] -> H = filters[q + 1] -> H;
		filters[q] -> W = filters[q + 1] -> W;
		filters[q] -> is_normalized = filters[q + 1] -> is_normalized;
	}

	free(filters[Fnum - 1]);	// 가장 마지막에 저장된 필터의 정보는 바로 앞칸으로 옮겼으므로 할당 해제하면 지울 필터만 정보가 없어져있음 
	filters = (CUBIC**)realloc(filters, sizeof(CUBIC*) * (Fnum - 1));	// 공간 압축 
	// 현재 포인터 배열의 공간이 압축된 상태. 하지만 선택한 필터보다 아래쪽에 있던 필터들의 pixel data가 사라진 상태이므로 이를 저장해주어야 함 
	for(q = select; q < Fnum - 1; q++) {	// pixel data 저장하기 
		strcpy(name, filters[q] -> name);	// 원본 데이터의 변경을 막기 위해 name으로 이름을 복사해 name을 편집 
		Plusppm(name, ppm);	// .ppm 추가 
		filfile = fopen(name, "r");
		if(filfile == NULL) {	// 파일 없을 때 
			printf("\aError : Failed to delete the filter.\n");	// 파일이 없으면 사라진 pixel data의 정보를 저장할 방법이 없으므로 프로그램 강제 종료
			printf("Forcibly terminates the program due to an error.\n");
			Quit(images, filters, imgnum, filnum);
		} 
		
		fscanf(filfile, "%*c%*d %*d %*d %*d\n");	// 필요한 것은 pixel data이므로 나머지는 버림 
		
		C = filters[q] -> C;
		H = filters[q] -> H;
		W = filters[q] -> W;
		
		filters[q] -> data = (float***)malloc(sizeof(float**) * C);	// pixel data 저장용 공간 할당받기 
		for(i = 0; i < C; i++) {
			filters[q] -> data[i] = (float**)malloc(sizeof(float*) * H);
			for(j = 0; j < H; j++) {
				filters[q] -> data[i][j] = (float*)malloc(sizeof(float) * W);
			}
		} 
		// 파일 구조를 고려하면 j, k, i 순서로 for문을 구성해야 함. 
		for(j = 0; j < H; j++) {	// 데이터 저장
			for(k = 0; k < W; k++) {
				for(i = 0; i < C; i++) {
					fscanf(filfile, "%f", &(filters[q] -> data[i][j][k]));
				}
			}
		}
	}
	
	Fnum -= 1;	// 필터 수 -1 
	*filnum = Fnum;
	
	return filters;
}

CUBIC** Printimg(CUBIC** images, int* imgnum) {
	int i, Ci, Hi, Wi, Inum;
	
	Inum = *imgnum;
	for(i = 0; i < Inum; i++) {	// 현재 목록에 있는 이미지 출력 
		Ci = images[i] -> C; 
		Hi = images[i] -> H;
		Wi = images[i] -> W;
		if(images[i] -> is_normalized == 1)	//정규화 된 상태일 경우 
		printf("%d. %s [H : %d, W : %d, C : %d] - %s\n", i, images[i] -> name, Hi, Wi, Ci, "normalize");
		else
		printf("%d. %s [H : %d, W : %d, C : %d]\n", i, images[i] -> name, Hi, Wi, Ci);
	}
	
	return images;
}

CUBIC** Printfil(CUBIC** filters, int* filnum) {
	int i, Cf, Hf, Wf, Fnum;
	
	Fnum = *filnum;	
	for(i = 0; i < Fnum; i++) {	// 현재 목록에 있는 필터 출력 
		Cf = filters[i] -> C;
		Hf = filters[i] -> H;
		Wf = filters[i] -> W;
		printf("%d. %s [H : %d, W : %d, C : %d]\n", i, filters[i] -> name, Hf, Wf, Cf);
	}
	
	return filters;
}

CUBIC** Normalize(CUBIC** images, int* imgnum) {
	int i, j, k, select, check = 0, Inum;	// i는 Rc, j는 Rh, k는 Rw에 대응. check는 normalize할 이미지가 있는지 검사용 
	int Rh, Rw, Rc; 
	Inum = *imgnum;
	
	for(i = 0; i < Inum; i++) {
		if(images[i] -> is_normalized == 0) {	// 정규화 되지 않은 이미지가 있을 경우 정규화 가능하므로 check를 1로 변경 후 break 
			check = 1; break;
		}
	}
	
	if(Inum == 0 || check != 1) {	// check가 1이 아니거나 이미지가 없으면 정규화 불가 
		printf("\aError : The image to be normalized is not in the list.\n");
		return images;
	}
	
	Printimg(images, imgnum);	// 이미지 목록 출력 
	printf("Enter tne number of image to normalize : ");
	scanf("%d", &select);
	
	if(select >= Inum || select < 0) {	// 목록에 없는 값 입력시 오류 
		error();
		return images;
	}
	
	if(images[select] -> is_normalized == 1) {	// 정규화된 이미지 선택시 오류 
		printf("Error : The image '%s' is not denormailzed. Please select a denormailzed image.\n", images[select] -> name);
		return images;
	}

	Rh = images[select] -> H;
	Rw = images[select] -> W;
	Rc = images[select] -> C;
	
	for(i = 0; i < Rc; i++) {	// 정규화. 모든 pixel data에 대해 진행하므로 순서 상관 x 
		for(j = 0; j < Rh; j++) {
			for(k = 0; k < Rw; k++) {
				images[select] -> data[i][j][k] = (images[select] -> data[i][j][k]) / 128 - 1;
			}
		}
	}	
	
	images[select] -> is_normalized = 1;	// 정규화했으므로 값을 1로 변경 
	
	return images;
}

CUBIC** Denormalize(CUBIC** images, int* imgnum) {
	int i, j, k, select, check = 0, Inum;	// i는 Rc, j는 Rh, k는 Rw에 대응. check는 denormalize할 이미지가 있는지 검사용 
	int Rh, Rw, Rc; 
	Inum = *imgnum;
	
	for(i = 0; i < Inum; i++) {	
		if(images[i] -> is_normalized == 1) {	// 정규화 된 이미지가 있을 경우 역정규화 가능하므로 check를 1로 변경 후 break
			check = 1; break;
		}
	}
	
	if(Inum == 0 || check != 1) {	// check가 1이 아니거나 이미지가 없으면 역정규화 불가 
		printf("\aError : The image to be denormalized is not in the list.\n");
		return images;
	}
	
	Printimg(images, imgnum);	// 이미지 목록 출력 
	printf("Enter tne number of image to denormalize : ");
	scanf("%d", &select);
	
	if(select >= Inum || select < 0) {	// 목록에 없는 값 입력시 오류 
		error();
		return images;
	}
	
	if(images[select] -> is_normalized == 0) {	// 정규화 되지 않은 이미지 선택시 오류
		printf("Error : The image '%s' is not normailzed. Please select a normailzed image.\n", images[select] -> name);
		return images;
	}

	Rh = images[select] -> H;
	Rw = images[select] -> W;
	Rc = images[select] -> C;

	for(i = 0; i < Rc; i++) {	// 역정규화. 모든 pixel data에 대해 진행하므로 순서 상관 x
		for(j = 0; j < Rh; j++) {
			for(k = 0; k < Rw; k++) {
				images[select] -> data[i][j][k] = ((images[select] -> data[i][j][k]) + 1) * 128;
			}
		}
	}
	
	images[select] -> is_normalized = 0;	// 역정규화했으므로 값을 0으로 변경 
	
	return images;
}

CUBIC** ImageConvol(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum) {
	int imgsel, filsel, i, j, k, ri, rj, rk, Inum, Fnum;	// ri는 Rc, rj는 Rh, rk는 Rw에 대응. i, j, k는 여러 for문에서 사용하나, channel, height, width에 대응되도록 구성 
	int Rh, Ih, Fh;	// height. result, image, filter 
	int Rw, Iw, Fw;	// width
	int Rc, Ic, Fc;	// channel
	char name[100];
	
	Inum = *imgnum;
	Fnum = *filnum;
	
	if(Inum == 0) {	// 이미지가 목록에 없을 때 
		printf("\aError : There are no images to convolve in the list.\n");
		return images;
	}
	if(Fnum == 0) {	// 필터가 목록에 없을 때 
		printf("\aError : There are no filters to convolve in the list.\n");
		return images;
	}
	
	Printimg(images, imgnum);	// 이미지 목록 출력 
	printf("Enter the number of image to convolve : ");
	scanf("%d", &imgsel);
	if(imgsel > Inum || imgsel < 0) {
		error();
		return images;
	}
	
	Printfil(filters, filnum);	// 필터 목록 출력 
	printf("Enter the number of filter to convolve : ");
	scanf("%d", &filsel);
	if(filsel > Fnum || filsel < 0) {
		error();
		return images;
	}
	
	images = (CUBIC**)realloc(images, sizeof(CUBIC*) * (Inum + 1));	// 결과 이미지 저장용 공간 할당 
	images[Inum] = (CUBIC*)malloc(sizeof(CUBIC));
	
	Ih = images[imgsel] -> H;	// convolution을 위해 데이터를 공식에 맞게 저장 
	Fh = filters[filsel] -> H;
	Rh = Ih - Fh + 1;
	Iw = images[imgsel] -> W;
	Fw = filters[filsel] -> W;
	Rw = Iw - Fw + 1;
	Ic = images[imgsel] -> C;
	Fc = filters[filsel] -> C;
	Rc = Ic;
	strcpy(name, images[imgsel] -> name);	// 원래 데이터 변경 방지를 위해 name에 복사 
	
	images[Inum] -> data = (float***)malloc(sizeof(float**) * Rc);	// pixel data 저장용 공간 할당받기. 필요한 만큼만 할당  
	for(i = 0; i < Rc; i++) {
		images[Inum] -> data[i] = (float**)malloc(sizeof(float*) * Rh);
		for(j = 0; j < Rh; j++) {
			images[Inum] -> data[i][j] = (float*)malloc(sizeof(float) * Rw);
		}
	}
	
	for(ri = 0; ri < Rc; ri++) {	// 오류 방지를 위해 모든 pixel data 0으로 초기화 
		for(rj = 0; rj < Rh; rj++) {
			for(rk = 0; rk < Rw; rk ++) {
				images[Inum] -> data[ri][rj][rk] = 0;
			}
		}
	}
	
	for(ri = 0; ri < Rc; ri++) {	// convolution 진행. 모든 pixel data에 대해 진행하므로 ri, rj, rk의 순서는 상관 x 
		for(rj = 0; rj < Rh; rj++) {
			for(rk = 0; rk < Rw; rk++) {
				for(j = 0; j < Fh; j++) {	// j, k는 convolution 공식에 따른 순서 
					for(k = 0; k < Fw; k++) {
						images[Inum] -> data[ri][rj][rk] += (images[imgsel] -> data[ri][rj + j][rk + k]) * (filters[filsel] -> data[0][j][k]);
					}
				}
			}
		}
	}
	
	strcpy(images[Inum] -> name, Plusstr(name, filters[filsel] -> name));	// 이름 형식에 맞게 저장 
	images[Inum] -> C = Rc;	// convolution한 이미지의 정보(channel, height, width, 정규화 여부)를 저장 
	images[Inum] -> H = Rh;
	images[Inum] -> W = Rw;
	
	if(images[imgsel] -> is_normalized == 1)	// 정규화 된 이미지로 작업했으면 결과는 정규화 상태 
	images[Inum] -> is_normalized = 1;
	else
	images[Inum] -> is_normalized = 0;	//정규화 되지 않은 이미지로 작업했으면 결과는 정규화가 안된 상태 
	
	Inum += 1;	// 이미지 수 +1 
	*imgnum = Inum;
	
	return images;
}

void Quit(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum) {
	int q, i, j, C, H, Inum, Fnum;
	
	Inum = *imgnum;
	Fnum = *filnum;
	
	for(q = 0; q < Inum; q++) {	// 프로그램 종료 전 모든 할당받은 이미지 공간 해제 
		C = images[q] -> C;
		H = images[q] -> H; 
		for(i = 0; i < C; i++) {
			for(j = 0; j < H; j++) {
				free(images[q] -> data[i][j]); 
			}
			free(images[q] -> data[i]);
		}
		free(images[q] -> data);
		free(images[q]);
	}
	free(images);
	
	for(q = 0; q < Fnum; q++) {	// 프로그램 종료 전 모든 할당받은 필터 공간 해제 
		C = filters[q] -> C;
		H = filters[q] -> H; 
		for(i = 0; i < C; i++) {
			for(j = 0; j < H; j++) {
				free(filters[q] -> data[i][j]);
			}
			free(filters[q] -> data[i]);
		}
		free(filters[q] -> data);
		free(filters[q]);
	}
	free(filters);
	
	exit(0);	//프로그램 종료 
}

void error() {
	printf("\aPlease select the correct menu.\nPress Enter to continue.");
	while (getchar() != '\n');
	getchar();	// Enter를 입력해 넘기기 
}

char* Delppm(char *str) {
	int i, count = 0;
	
	for(i = 0; str[i] != '\0'; i++)
	count++;	// str의 문자 수 세기 
	
	str[count - 4] = '\0';	//".ppm"은 4글자 이므로 끝에서 4칸 뺀 곳에 NULL을 입력하면 "abc.ppm"이 "abc"로 바뀜 

	return str;
}

char* Plusppm(char *str, char *ppm) {
	int i, count = 0;
	
	for(i = 0; str[i] != '\0'; i++)
	count++;	// str의 문자 수 세기 
	
	for(i = 0; str[i] != '\0'; i++)
	str[count + i] =  ppm[i];	// 문자열 이어붙이기 
	
	str[count + i] = '\0';	// 문자의 마지막은 NULL 
	
	return str;	// 시작주소 반환 
}

char* Plusstr(char *str1, char *str2) {	// Plusppm과 비슷한 구조로 만들어졌으나, "_" 추가를 위해 새로운 함수 작성 
	int i, count = 0;
	
	for(i = 0; str1[i] != '\0'; i++)
	count++;	// str1의 문자 수 세기 
	
	str1[i] = '_';	// 끝에 NULL 대신 "_" 저장 
	for(i = 0; str2[i] != '\0'; i++)
	str1[count + i + 1] =  str2[i];		// "_" 뒤에 str2 문자열 이어붙이기 
	
	str1[count + i + 1] = '\0';	// 문자의 마지막은 NULL 
	
	return str1;	// 시작주소 반환 
}
