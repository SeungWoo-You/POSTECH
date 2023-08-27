#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"

CUBIC** ImageLoad(CUBIC** images, int* imgnum) {
	FILE* imgfile;
	int i, j, k, Inum;	// i�� C, j�� H, k�� W�� ���� 
	int C, H, W;	// channal, height, width
	
	Inum = *imgnum; 
	
	if(Inum  == 0)
	images = (CUBIC**)malloc(sizeof(CUBIC*));	// ��Ͽ� �̹����� ������ ó������ �Ҵ� 
	else
	images = (CUBIC**)realloc(images, sizeof(CUBIC*) * (Inum + 1));	// ��Ͽ� �̹����� ������ �߰����� �Ҵ�ޱ� 
	
	images[Inum] = (CUBIC*)malloc(sizeof(CUBIC));
	printf("Enter image filename : ");
	scanf("%s", images[Inum] -> name);	// �̸� ���� 
	
	imgfile = fopen(images[Inum] -> name, "r");
	if(imgfile == NULL) {	// �ش� ���� ���� �� 
		printf("\aError : The file '%s' does not exist.\n", images[Inum] -> name);
		free(images[Inum]);	// ����Ǵ� ������ �����Ƿ� �Ҵ���� ���� ���� 
		return images;
	}

	fscanf(imgfile, "%*c%d %d %d %*d\n", &images[Inum] -> C, &images[Inum] -> W, &images[Inum]-> H);	// P3 600 370 255���� P�� 255�� �ʿ���� �����̹Ƿ� ���� 
	C = images[Inum] -> C;
	H = images[Inum] -> H;
	W = images[Inum] -> W;
	
	images[Inum] -> data = (float***)malloc(sizeof(float**) * C);	// pixel data ����� ���� �Ҵ�ޱ�. �ʿ��� ��ŭ�� �Ҵ� 
	for(i = 0; i < C; i++) {
		images[Inum] -> data[i] = (float**)malloc(sizeof(float*) * H);
		for(j = 0; j < H; j++) {
			images[Inum] -> data[i][j] = (float*)malloc(sizeof(float) * W);
		}
	}
	// ���� ������ ����ϸ� j(H), k(W), i(C) ������ for���� �����ؾ� �� 
	for(j = 0; j < H; j++) {	// ������ ����
		for(k = 0; k < W; k++) {
			for(i = 0; i < C; i++) {
				fscanf(imgfile, "%f", &(images[Inum] -> data[i][j][k]));
			}
		}
	}
	
	images[Inum] -> is_normalized = 0;	// ó������ ����ȭ�� �Ǿ����� �����Ƿ� 0 
	
	Delppm(images[Inum] -> name);	// �̸����� .ppm ����
	
	Inum += 1;	// �̹��� �� +1
	*imgnum = Inum;
	
	fclose(imgfile);
	
	return images;
}

CUBIC** ImageSave(CUBIC** images, int* imgnum) {
	FILE* imgfile;
	int i, j, k, select, Inum;	// i�� Ci, j�� Hi, k�� Wi�� ���� 
	int Ci, Hi, Wi;	// channel, height, width
	char name[100];	// �̸� ����� 
	
	Inum = *imgnum;
	if(Inum == 0) {	// �̹��� ���� �� 
		printf("\aError : There are no images to save in the list.\n");
		return images;
	}
	Printimg(images, imgnum);	// �̹��� ��� ��� 
	
	printf("Enter the number of image to save : ");
		scanf("%d", &select);
		if(select >= Inum || select < 0) {	// ��Ͽ� ���� �� �Է½� ���� 
			error(); return images;
	}
	
	Ci = images[select] -> C;
	Hi = images[select] -> H;
	Wi = images[select] -> W;
	
	printf("Enter filename for save image : ");
	scanf("%s", name);
	
	imgfile = fopen(name, "w");	// ���� ���� �Է� 
	if(imgfile == NULL) {	// ���� ������ ������ ��� 
		printf("\aError : Cannot create the image'%s'\n", name);
		return images;
	}
	
	fprintf(imgfile, "%c%d\n", 'P', Ci);	// ���� ���� ��� 
	fprintf(imgfile, "%d %d\n" , Wi, Hi);
	fprintf(imgfile, "%d\n", 255);
	
	for(j = 0; j < Hi; j++) {	// ������ ����. ���� ���� ����� j(Hi), k(Wi), i(Ci) ������ for�� ����
		for(k = 0; k < Wi; k++) {
			for(i = 0; i < Ci; i++) {
				if((int)(images[select] -> data[i][j][k]) < 0) 
				fprintf(imgfile, "%d ", 0);
				else if((int)(images[select] -> data[i][j][k]) > 255)
				fprintf(imgfile, "%d ", 255);
				else
				fprintf(imgfile, "%d ",(int)(images[select] -> data[i][j][k]));	// ������ ���� 
			}
		}
	}
	
	fclose(imgfile);
	
	return images;
}

CUBIC** ImageRemove(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum) {
	FILE* imgfile;
	int q, i, j, k, select, Inum; // i�� C, j�� H, k�� W�� ���� 
	int C, H, W;
	char name[100], ppm[5] = ".ppm";	// name�� ���� ������ ������ ���� ����, ppm�� ".ppm" �߰��� ���� 
	
	Inum = *imgnum;
	if(Inum == 0) {	// �̹��� ���� �� 
		printf("\aError : There are no images to erase in the list.\n");
		return images;
	}
	
	Printimg(images, imgnum);	// �̹��� ��� ��� 
	printf("Enter the number of image to remove : ");
	scanf("%d", &select);
	
	if(select >= Inum || select < 0) {	// ��Ͽ� ���� �� �Է½� ���� 
		error(); return images;
	}
	
	for(q = select; q < Inum; q++) {	// ������ �̹����� ������ �̺��� ����� �Ʒ��ʿ� �ִ� �̹������� pixel data �Ҵ� ���� 
		C = images[q] -> C;				// �ʿ��� ��ŭ�� ������ �Ҵ�޾����Ƿ� �̹��������� �����ϱ� ���ؼ��� pixel data�� �Ҵ� ���� �� �� ���� �ʿ��� ��ŭ �Ҵ�޾ƾ� �� 
		H = images[q] -> H;				// �̷��� ���ϸ� �Ҵ���� ���� ������ �����ϰ� �� ���� ���� 
		for(i = 0; i < C; i++) {
			for(j = 0; j < H; j++) {
				free(images[q] -> data[i][j]);
			}
			free(images[q] -> data[i]);
		}
		free(images[q] -> data);
	}
	 
	for(q = select; q < Inum - 1; q++) {	// pixel data�� ������ �������� �״�� �����ص� ������. ��� ������ ������ �Ҵ����� �ϱ� ���� �����ؾ� �� 
		strcpy(images[q] -> name, images[q + 1] -> name);	// ���� �̹����� �������� ��ĭ �ڿ� �ִ� �̹����� ������ ������
		images[q] -> C = images[q + 1] -> C;
		images[q] -> H = images[q + 1] -> H;
		images[q] -> W = images[q + 1] -> W;
		images[q] -> is_normalized = images[q + 1] -> is_normalized;
	}
	
	free(images[Inum - 1]);	// ���� �������� ����� �̹����� ������ �ٷ� ��ĭ���� �Ű����Ƿ� �Ҵ� �����ϸ� ���� �̹����� ������ ���������� 
	images = (CUBIC**)realloc(images, sizeof(CUBIC*) * (Inum - 1));	// ���� ���� 
	// ���� ������ �迭�� ������ ����� ����. ������ ������ �̹������� �Ʒ��ʿ� �ִ� �̹������� pixel data�� ����� �����̹Ƿ� �̸� �������־�� �� 
	for(q = select; q < Inum - 1; q++) {	// pixel data �����ϱ� 
		strcpy(name, images[q] -> name);	// ���� �������� ������ ���� ���� name���� �̸��� ������ name�� ���� 
		Plusppm(name, ppm);	// .ppm �߰� 
		imgfile = fopen(name, "r");
		if(imgfile == NULL) {	// ���� ���� �� 
			printf("\aError : Failed to delete the image.\n");	// ������ ������ ����� pixel data�� ������ ������ ����� �����Ƿ� ���α׷� ���� ����
			printf("Forcibly terminates the program due to an error.\n");
			Quit(images, filters, imgnum, filnum);
		} 
		
		fscanf(imgfile, "%*c%*d %*d %*d %*d\n");	// �ʿ��� ���� pixel data�̹Ƿ� �������� ���� 
		
		C = images[q] -> C;
		H = images[q] -> H;
		W = images[q] -> W;
		
		images[q] -> data = (float***)malloc(sizeof(float**) * C);	// pixel data ����� ���� �Ҵ�ޱ� 
		for(i = 0; i < C; i++) {
			images[q] -> data[i] = (float**)malloc(sizeof(float*) * H);
			for(j = 0; j < H; j++) {
				images[q] -> data[i][j] = (float*)malloc(sizeof(float) * W);
			}
		} 
		// ���� ������ ����ϸ� j, k, i ������ for���� �����ؾ� ��. 
		for(j = 0; j < H; j++) {	// ������ ����
			for(k = 0; k < W; k++) {
				for(i = 0; i < C; i++) {
					fscanf(imgfile, "%f", &(images[q] -> data[i][j][k]));
				}
			}
		}
	}
	
	Inum -= 1;	// �̹��� �� -1 
	*imgnum = Inum;
	
	return images;
}

CUBIC** FilterLoad(CUBIC** filters, int* filnum) {	// ImageLoad �Լ��� ����� �������� ���� 
	FILE* filfile;
	int i, j, k, Fnum;	// i�� C, j�� H, k�� W�� ���� 
	int C, H, W;
	
	Fnum = *filnum;
	if(Fnum  == 0)
	filters = (CUBIC**)malloc(sizeof(CUBIC*));	// ��Ͽ� ���Ͱ� ������ ó������ �Ҵ� 
	else
	filters = (CUBIC**)realloc(filters, (Fnum + 1) * sizeof(CUBIC*));	// ��Ͽ� ���Ͱ� ������ �߰����� �Ҵ�ޱ� 
	
	printf("Enter filter filename : ");
	filters[Fnum] = (CUBIC*)malloc(sizeof(CUBIC));
	scanf("%s", filters[Fnum] -> name);	// �̸� ���� 
	
	filfile = fopen(filters[Fnum] -> name, "r");
	if(filfile == NULL) {	// �ش� ���� ���� �� 
		printf("\aError : The file '%s' does not exist.\n", filters[Fnum] -> name);
		free(filters[Fnum]);	// ����Ǵ� ������ �����Ƿ� �Ҵ���� ���� ���� 
		return filters;
	}
	
	fscanf(filfile, "%*c%d %d %d %*d\n", &filters[Fnum] -> C, &filters[Fnum] -> W, &filters[Fnum]-> H);	// P1 3 2 255���� P�� 255�� �ʿ���� �����̹Ƿ� ���� 
	C = filters[Fnum] -> C;
	H = filters[Fnum] -> H;
	W = filters[Fnum] -> W;
		
	filters[Fnum] -> data = (float***)malloc(sizeof(float**) * C);	// pixel data ����� ���� �Ҵ�ޱ�. �ʿ��� ��ŭ�� �Ҵ� 
	for(i = 0; i < C; i++) {
		filters[Fnum] -> data[i] = (float**)malloc(sizeof(float*) * H);
		for(j = 0; j < H; j++) {
			filters[Fnum] -> data[i][j] = (float*)malloc(sizeof(float) * W);
		}
	}
	// ���� ������ ����ϸ� j(H), k(W), i(C) ������ for���� �����ؾ� �� 
	for(j = 0; j < H; j++) {	// ������ ����
		for(k = 0; k < W; k++) {
			for(i = 0; i < C; i++) {
				fscanf(filfile, "%f", &(filters[Fnum] -> data[i][j][k]));
			}
		}
	}
	
	Delppm(filters[Fnum] -> name);	// �̸����� .ppm ����
	
	Fnum += 1;	// ���ͼ� +1 
	*filnum = Fnum;
	
	fclose(filfile);
	
	return filters;
}

CUBIC** FilterRemove(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum) {
	FILE* filfile;
	int q, i, j, k, select, Fnum;	// i�� C, j�� H, k�� W�� ����
	int  C, H, W; 
	char name[100], ppm[5] = ".ppm";	// name�� ���� ������ ������ ���� ����, ppm�� ".ppm" �߰��� ���� 
	
	Fnum = *filnum;
	if(Fnum == 0) {	// ���� ���� �� 
		printf("\aError : There are no filters to erase in the list.\n");
		return filters;
	}
	
	Printfil(filters, filnum);	// ���� ��� ��� 
	printf("Enter the number of filters to remove : ");
	scanf("%d", &select);
	
	if(select >= Fnum || select < 0) {	// ��Ͽ� ���� �� �Է½� ���� 
		error(); return filters;
	}
	
	for(q = select; q < Fnum; q++) {	// ������ ���͸� ������ �̺��� ����� �Ʒ��ʿ� �ִ� ���͵��� pixel data �Ҵ� ����
		C = filters[q] -> C;			// �ʿ��� ��ŭ�� ������ �Ҵ�޾����Ƿ� ���Ͱ����� �����ϱ� ���ؼ��� pixel data�� �Ҵ� ���� �� �� ���� �ʿ��� ��ŭ �Ҵ�޾ƾ� �� 
		H = filters[q] -> H; 			// �̷��� ���ϸ� �Ҵ���� ���� ������ �����ϰ� �� ���� ���� 
		for(i = 0; i < C; i++) {
			for(j = 0; j < H; j++) {
				free(filters[q] -> data[i][j]);
			}
			free(filters[q] -> data[i]);
		}
		free(filters[q] -> data);
	}
	
	for(q = select; q < Fnum - 1; q++) {	// pixel data�� ������ �������� �״�� �����ص� ������. ��� ������ ������ �Ҵ����� �ϱ� ���� �����ؾ� �� 
		strcpy(filters[q] -> name, filters[q + 1] -> name);	// ���� ���͸� �������� ��ĭ �ڿ� �ִ� ������ ������ ������
		filters[q] -> C = filters[q + 1] -> C;
		filters[q] -> H = filters[q + 1] -> H;
		filters[q] -> W = filters[q + 1] -> W;
		filters[q] -> is_normalized = filters[q + 1] -> is_normalized;
	}

	free(filters[Fnum - 1]);	// ���� �������� ����� ������ ������ �ٷ� ��ĭ���� �Ű����Ƿ� �Ҵ� �����ϸ� ���� ���͸� ������ ���������� 
	filters = (CUBIC**)realloc(filters, sizeof(CUBIC*) * (Fnum - 1));	// ���� ���� 
	// ���� ������ �迭�� ������ ����� ����. ������ ������ ���ͺ��� �Ʒ��ʿ� �ִ� ���͵��� pixel data�� ����� �����̹Ƿ� �̸� �������־�� �� 
	for(q = select; q < Fnum - 1; q++) {	// pixel data �����ϱ� 
		strcpy(name, filters[q] -> name);	// ���� �������� ������ ���� ���� name���� �̸��� ������ name�� ���� 
		Plusppm(name, ppm);	// .ppm �߰� 
		filfile = fopen(name, "r");
		if(filfile == NULL) {	// ���� ���� �� 
			printf("\aError : Failed to delete the filter.\n");	// ������ ������ ����� pixel data�� ������ ������ ����� �����Ƿ� ���α׷� ���� ����
			printf("Forcibly terminates the program due to an error.\n");
			Quit(images, filters, imgnum, filnum);
		} 
		
		fscanf(filfile, "%*c%*d %*d %*d %*d\n");	// �ʿ��� ���� pixel data�̹Ƿ� �������� ���� 
		
		C = filters[q] -> C;
		H = filters[q] -> H;
		W = filters[q] -> W;
		
		filters[q] -> data = (float***)malloc(sizeof(float**) * C);	// pixel data ����� ���� �Ҵ�ޱ� 
		for(i = 0; i < C; i++) {
			filters[q] -> data[i] = (float**)malloc(sizeof(float*) * H);
			for(j = 0; j < H; j++) {
				filters[q] -> data[i][j] = (float*)malloc(sizeof(float) * W);
			}
		} 
		// ���� ������ ����ϸ� j, k, i ������ for���� �����ؾ� ��. 
		for(j = 0; j < H; j++) {	// ������ ����
			for(k = 0; k < W; k++) {
				for(i = 0; i < C; i++) {
					fscanf(filfile, "%f", &(filters[q] -> data[i][j][k]));
				}
			}
		}
	}
	
	Fnum -= 1;	// ���� �� -1 
	*filnum = Fnum;
	
	return filters;
}

CUBIC** Printimg(CUBIC** images, int* imgnum) {
	int i, Ci, Hi, Wi, Inum;
	
	Inum = *imgnum;
	for(i = 0; i < Inum; i++) {	// ���� ��Ͽ� �ִ� �̹��� ��� 
		Ci = images[i] -> C; 
		Hi = images[i] -> H;
		Wi = images[i] -> W;
		if(images[i] -> is_normalized == 1)	//����ȭ �� ������ ��� 
		printf("%d. %s [H : %d, W : %d, C : %d] - %s\n", i, images[i] -> name, Hi, Wi, Ci, "normalize");
		else
		printf("%d. %s [H : %d, W : %d, C : %d]\n", i, images[i] -> name, Hi, Wi, Ci);
	}
	
	return images;
}

CUBIC** Printfil(CUBIC** filters, int* filnum) {
	int i, Cf, Hf, Wf, Fnum;
	
	Fnum = *filnum;	
	for(i = 0; i < Fnum; i++) {	// ���� ��Ͽ� �ִ� ���� ��� 
		Cf = filters[i] -> C;
		Hf = filters[i] -> H;
		Wf = filters[i] -> W;
		printf("%d. %s [H : %d, W : %d, C : %d]\n", i, filters[i] -> name, Hf, Wf, Cf);
	}
	
	return filters;
}

CUBIC** Normalize(CUBIC** images, int* imgnum) {
	int i, j, k, select, check = 0, Inum;	// i�� Rc, j�� Rh, k�� Rw�� ����. check�� normalize�� �̹����� �ִ��� �˻�� 
	int Rh, Rw, Rc; 
	Inum = *imgnum;
	
	for(i = 0; i < Inum; i++) {
		if(images[i] -> is_normalized == 0) {	// ����ȭ ���� ���� �̹����� ���� ��� ����ȭ �����ϹǷ� check�� 1�� ���� �� break 
			check = 1; break;
		}
	}
	
	if(Inum == 0 || check != 1) {	// check�� 1�� �ƴϰų� �̹����� ������ ����ȭ �Ұ� 
		printf("\aError : The image to be normalized is not in the list.\n");
		return images;
	}
	
	Printimg(images, imgnum);	// �̹��� ��� ��� 
	printf("Enter tne number of image to normalize : ");
	scanf("%d", &select);
	
	if(select >= Inum || select < 0) {	// ��Ͽ� ���� �� �Է½� ���� 
		error();
		return images;
	}
	
	if(images[select] -> is_normalized == 1) {	// ����ȭ�� �̹��� ���ý� ���� 
		printf("Error : The image '%s' is not denormailzed. Please select a denormailzed image.\n", images[select] -> name);
		return images;
	}

	Rh = images[select] -> H;
	Rw = images[select] -> W;
	Rc = images[select] -> C;
	
	for(i = 0; i < Rc; i++) {	// ����ȭ. ��� pixel data�� ���� �����ϹǷ� ���� ��� x 
		for(j = 0; j < Rh; j++) {
			for(k = 0; k < Rw; k++) {
				images[select] -> data[i][j][k] = (images[select] -> data[i][j][k]) / 128 - 1;
			}
		}
	}	
	
	images[select] -> is_normalized = 1;	// ����ȭ�����Ƿ� ���� 1�� ���� 
	
	return images;
}

CUBIC** Denormalize(CUBIC** images, int* imgnum) {
	int i, j, k, select, check = 0, Inum;	// i�� Rc, j�� Rh, k�� Rw�� ����. check�� denormalize�� �̹����� �ִ��� �˻�� 
	int Rh, Rw, Rc; 
	Inum = *imgnum;
	
	for(i = 0; i < Inum; i++) {	
		if(images[i] -> is_normalized == 1) {	// ����ȭ �� �̹����� ���� ��� ������ȭ �����ϹǷ� check�� 1�� ���� �� break
			check = 1; break;
		}
	}
	
	if(Inum == 0 || check != 1) {	// check�� 1�� �ƴϰų� �̹����� ������ ������ȭ �Ұ� 
		printf("\aError : The image to be denormalized is not in the list.\n");
		return images;
	}
	
	Printimg(images, imgnum);	// �̹��� ��� ��� 
	printf("Enter tne number of image to denormalize : ");
	scanf("%d", &select);
	
	if(select >= Inum || select < 0) {	// ��Ͽ� ���� �� �Է½� ���� 
		error();
		return images;
	}
	
	if(images[select] -> is_normalized == 0) {	// ����ȭ ���� ���� �̹��� ���ý� ����
		printf("Error : The image '%s' is not normailzed. Please select a normailzed image.\n", images[select] -> name);
		return images;
	}

	Rh = images[select] -> H;
	Rw = images[select] -> W;
	Rc = images[select] -> C;

	for(i = 0; i < Rc; i++) {	// ������ȭ. ��� pixel data�� ���� �����ϹǷ� ���� ��� x
		for(j = 0; j < Rh; j++) {
			for(k = 0; k < Rw; k++) {
				images[select] -> data[i][j][k] = ((images[select] -> data[i][j][k]) + 1) * 128;
			}
		}
	}
	
	images[select] -> is_normalized = 0;	// ������ȭ�����Ƿ� ���� 0���� ���� 
	
	return images;
}

CUBIC** ImageConvol(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum) {
	int imgsel, filsel, i, j, k, ri, rj, rk, Inum, Fnum;	// ri�� Rc, rj�� Rh, rk�� Rw�� ����. i, j, k�� ���� for������ ����ϳ�, channel, height, width�� �����ǵ��� ���� 
	int Rh, Ih, Fh;	// height. result, image, filter 
	int Rw, Iw, Fw;	// width
	int Rc, Ic, Fc;	// channel
	char name[100];
	
	Inum = *imgnum;
	Fnum = *filnum;
	
	if(Inum == 0) {	// �̹����� ��Ͽ� ���� �� 
		printf("\aError : There are no images to convolve in the list.\n");
		return images;
	}
	if(Fnum == 0) {	// ���Ͱ� ��Ͽ� ���� �� 
		printf("\aError : There are no filters to convolve in the list.\n");
		return images;
	}
	
	Printimg(images, imgnum);	// �̹��� ��� ��� 
	printf("Enter the number of image to convolve : ");
	scanf("%d", &imgsel);
	if(imgsel > Inum || imgsel < 0) {
		error();
		return images;
	}
	
	Printfil(filters, filnum);	// ���� ��� ��� 
	printf("Enter the number of filter to convolve : ");
	scanf("%d", &filsel);
	if(filsel > Fnum || filsel < 0) {
		error();
		return images;
	}
	
	images = (CUBIC**)realloc(images, sizeof(CUBIC*) * (Inum + 1));	// ��� �̹��� ����� ���� �Ҵ� 
	images[Inum] = (CUBIC*)malloc(sizeof(CUBIC));
	
	Ih = images[imgsel] -> H;	// convolution�� ���� �����͸� ���Ŀ� �°� ���� 
	Fh = filters[filsel] -> H;
	Rh = Ih - Fh + 1;
	Iw = images[imgsel] -> W;
	Fw = filters[filsel] -> W;
	Rw = Iw - Fw + 1;
	Ic = images[imgsel] -> C;
	Fc = filters[filsel] -> C;
	Rc = Ic;
	strcpy(name, images[imgsel] -> name);	// ���� ������ ���� ������ ���� name�� ���� 
	
	images[Inum] -> data = (float***)malloc(sizeof(float**) * Rc);	// pixel data ����� ���� �Ҵ�ޱ�. �ʿ��� ��ŭ�� �Ҵ�  
	for(i = 0; i < Rc; i++) {
		images[Inum] -> data[i] = (float**)malloc(sizeof(float*) * Rh);
		for(j = 0; j < Rh; j++) {
			images[Inum] -> data[i][j] = (float*)malloc(sizeof(float) * Rw);
		}
	}
	
	for(ri = 0; ri < Rc; ri++) {	// ���� ������ ���� ��� pixel data 0���� �ʱ�ȭ 
		for(rj = 0; rj < Rh; rj++) {
			for(rk = 0; rk < Rw; rk ++) {
				images[Inum] -> data[ri][rj][rk] = 0;
			}
		}
	}
	
	for(ri = 0; ri < Rc; ri++) {	// convolution ����. ��� pixel data�� ���� �����ϹǷ� ri, rj, rk�� ������ ��� x 
		for(rj = 0; rj < Rh; rj++) {
			for(rk = 0; rk < Rw; rk++) {
				for(j = 0; j < Fh; j++) {	// j, k�� convolution ���Ŀ� ���� ���� 
					for(k = 0; k < Fw; k++) {
						images[Inum] -> data[ri][rj][rk] += (images[imgsel] -> data[ri][rj + j][rk + k]) * (filters[filsel] -> data[0][j][k]);
					}
				}
			}
		}
	}
	
	strcpy(images[Inum] -> name, Plusstr(name, filters[filsel] -> name));	// �̸� ���Ŀ� �°� ���� 
	images[Inum] -> C = Rc;	// convolution�� �̹����� ����(channel, height, width, ����ȭ ����)�� ���� 
	images[Inum] -> H = Rh;
	images[Inum] -> W = Rw;
	
	if(images[imgsel] -> is_normalized == 1)	// ����ȭ �� �̹����� �۾������� ����� ����ȭ ���� 
	images[Inum] -> is_normalized = 1;
	else
	images[Inum] -> is_normalized = 0;	//����ȭ ���� ���� �̹����� �۾������� ����� ����ȭ�� �ȵ� ���� 
	
	Inum += 1;	// �̹��� �� +1 
	*imgnum = Inum;
	
	return images;
}

void Quit(CUBIC** images, CUBIC** filters, int* imgnum, int* filnum) {
	int q, i, j, C, H, Inum, Fnum;
	
	Inum = *imgnum;
	Fnum = *filnum;
	
	for(q = 0; q < Inum; q++) {	// ���α׷� ���� �� ��� �Ҵ���� �̹��� ���� ���� 
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
	
	for(q = 0; q < Fnum; q++) {	// ���α׷� ���� �� ��� �Ҵ���� ���� ���� ���� 
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
	
	exit(0);	//���α׷� ���� 
}

void error() {
	printf("\aPlease select the correct menu.\nPress Enter to continue.");
	while (getchar() != '\n');
	getchar();	// Enter�� �Է��� �ѱ�� 
}

char* Delppm(char *str) {
	int i, count = 0;
	
	for(i = 0; str[i] != '\0'; i++)
	count++;	// str�� ���� �� ���� 
	
	str[count - 4] = '\0';	//".ppm"�� 4���� �̹Ƿ� ������ 4ĭ �� ���� NULL�� �Է��ϸ� "abc.ppm"�� "abc"�� �ٲ� 

	return str;
}

char* Plusppm(char *str, char *ppm) {
	int i, count = 0;
	
	for(i = 0; str[i] != '\0'; i++)
	count++;	// str�� ���� �� ���� 
	
	for(i = 0; str[i] != '\0'; i++)
	str[count + i] =  ppm[i];	// ���ڿ� �̾���̱� 
	
	str[count + i] = '\0';	// ������ �������� NULL 
	
	return str;	// �����ּ� ��ȯ 
}

char* Plusstr(char *str1, char *str2) {	// Plusppm�� ����� ������ �����������, "_" �߰��� ���� ���ο� �Լ� �ۼ� 
	int i, count = 0;
	
	for(i = 0; str1[i] != '\0'; i++)
	count++;	// str1�� ���� �� ���� 
	
	str1[i] = '_';	// ���� NULL ��� "_" ���� 
	for(i = 0; str2[i] != '\0'; i++)
	str1[count + i + 1] =  str2[i];		// "_" �ڿ� str2 ���ڿ� �̾���̱� 
	
	str1[count + i + 1] = '\0';	// ������ �������� NULL 
	
	return str1;	// �����ּ� ��ȯ 
}
