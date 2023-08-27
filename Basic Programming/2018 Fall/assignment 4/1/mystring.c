#include <stddef.h>

int mystrlen(char *str) {
	int i, count = 0;
	
	for(i = 0; str[i] != '\0'; i++)	// ���ڰ� ���� �� ������ �ݺ� 
	count++;	// ���ڰ� �ִٸ� count�� 1�� �ø�
	
	return count;
}

char *mystrcpy(char *toStr, char *fromStr) {
	int i;
	
	for(i = 0; fromStr[i] != '\0'; i++)	// ���ڰ� ���� �� ������ �ݺ� 
	toStr[i] = fromStr[i];	// fromStr�� ���ڸ� toStr�� ���� 
	
	toStr[i] = '\0';	// ���ڿ��� �������� NULL
	
	return toStr;
}

char *mystrcat(char *str1, char *str2) {
	int i, count = 0;
	
	for(i = 0; str1[i] != '\0'; i++)	// str1 ���� ���� ����
	count++;
	
	for(i = 0; str2[i] != '\0'; i++)
	str1[count + i] =  str2[i];		// ���ڿ� �̾���̱�. str1[count] = NULL�̹Ƿ� NULL���ڸ� str2�� ù ��° ���ڷ� ���� ��. 
	
	str1[count + i] = '\0';		// ���ڿ��� �������� NULL 
	
	return str1;
}

char *mystrchr(char *str, char c) {
	int i;
	
	for(i = 0; str[i] != '\0'; i++) {	// ���ڿ� �˻� 
		if(str[i] == c)		// str���� c�� ��ġ�ϴ� ���� �ִٸ� �� ���� �ּ� ��ȯ 
		return str + i;
	}
	
	return NULL;	// ������ NULL ��ȯ 
}

