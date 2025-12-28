#include <MDE/Core/String.h>

#include <stb_ds.h>

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

char** MDEStringToExec(const char* exec, const char* file) {
	char** a = NULL;
	int    i;
	char*  buf = malloc(1);

	buf[0] = 0;

	for(i = 0;; i++) {
		if(exec[i] == ' ' || exec[i] == 0) {
			if(strlen(buf) > 0) arrput(a, buf);

			buf    = malloc(1);
			buf[0] = 0;

			if(exec[i] == 0) break;
		} else if(exec[i] == '%') {
			char c = exec[++i];

			if(c == 'f' && file != NULL) {
				char* old = buf;

				buf = malloc(strlen(old) + strlen(file) + 1);
				strcpy(buf, old);
				strcpy(buf + strlen(old), file);

				free(old);
			}
		} else {
			char* old = buf;

			buf = malloc(strlen(old) + 2);
			strcpy(buf, old);
			buf[strlen(old)]     = exec[i];
			buf[strlen(old) + 1] = 0;

			free(old);
		}
	}

	arrput(a, NULL);

	return a;
}
