#include <stddef.h>

int mystrlen(char *str) { 
	int i, count = 0;

	for (i = 0; str[i] != '\0'; i++)
		count++;

	return count;
}

char *mystrcpy(char *toStr, char *fromStr) {
	int i;

	for (i = 0; fromStr[i] != '\0'; i++)
		toStr[i] = fromStr[i];
	
	toStr[i] = '\0';

	return toStr;
}

int mystrcmp(char *str1, char *str2) {
	int i;

	if (mystrlen(str1) < mystrlen(str2))
		return -1;
	else if (mystrlen(str1) > mystrlen(str2))
		return 1;


	for (i = 0; str1[i] != '\0'; i++) {
		if (str1[i] < str2[i])
			return -1;
		else if (str1[i] > str2[i])
			return 1;
	}

	return 0;
}

char *mystrcat(char *dest, char *src) {
	int i, count = 0;

	for (i = 0; dest[i] != '\0'; i++)
		count++;
	
	for (i = 0; src[i] != '\0'; i++)
		dest[count + i] = src[i];

	dest[count + i] = '\0';

	return dest;
}

char *mystrrchr(char *str, char c) {
	int i;

	i = mystrlen(str);
	i -= 1;
	for (i; i >= 0; i--) {
		if (str[i] == c)
			return str + i;
	}

	return NULL;
}
