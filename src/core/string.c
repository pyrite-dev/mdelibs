#include <MDE/Core/String.h>

char* MDEStringDuplicate(const char* src) {
	char* s = malloc(strlen(src) + 1);

	strcpy(s, src);

	return s;
}
