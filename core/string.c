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

char* MDEStringConcatenate3(const char* str1, const char* str2, const char* str3) {
	char* r = malloc(strlen(str1) + strlen(str2) + strlen(str3) + 1);
	strcpy(r, str1);
	strcat(r, str2);
	strcat(r, str3);

	return r;
}

char* MDEStringConcatenate4(const char* str1, const char* str2, const char* str3, const char* str4) {
	char* r = malloc(strlen(str1) + strlen(str2) + strlen(str3) + strlen(str4) + 1);
	strcpy(r, str1);
	strcat(r, str2);
	strcat(r, str3);
	strcat(r, str4);

	return r;
}
