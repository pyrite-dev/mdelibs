#include <MDE/Core/String.h>

char* MDEStringDuplicate(const char* src) {
	char* s = malloc(strlen(src) + 1);

	strcpy(s, src);

	return s;
}

char* MDEStringConcatenate(const char* str1, const char* str2) {
	char* r = malloc(strlen(str1) + strlen(str2) + 1);
	strcpy(r, str1);
	strcat(r, str2);

	return r;
}
