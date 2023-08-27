#include <stddef.h>

int mystrlen(char *str) {
	int i, count = 0;
	
	for(i = 0; str[i] != '\0'; i++)	// 문자가 있을 때 까지만 반복 
	count++;	// 문자가 있다면 count를 1씩 올림
	
	return count;
}

char *mystrcpy(char *toStr, char *fromStr) {
	int i;
	
	for(i = 0; fromStr[i] != '\0'; i++)	// 문자가 있을 때 까지만 반복 
	toStr[i] = fromStr[i];	// fromStr의 문자를 toStr에 복사 
	
	toStr[i] = '\0';	// 문자열의 마지막은 NULL
	
	return toStr;
}

char *mystrcat(char *str1, char *str2) {
	int i, count = 0;
	
	for(i = 0; str1[i] != '\0'; i++)	// str1 문자 개수 세기
	count++;
	
	for(i = 0; str2[i] != '\0'; i++)
	str1[count + i] =  str2[i];		// 문자열 이어붙이기. str1[count] = NULL이므로 NULL문자를 str2의 첫 번째 문자로 덮어 씀. 
	
	str1[count + i] = '\0';		// 문자열의 마지막은 NULL 
	
	return str1;
}

char *mystrchr(char *str, char c) {
	int i;
	
	for(i = 0; str[i] != '\0'; i++) {	// 문자열 검사 
		if(str[i] == c)		// str에서 c와 일치하는 곳이 있다면 그 시작 주소 반환 
		return str + i;
	}
	
	return NULL;	// 없으면 NULL 반환 
}

