#include <MDE/String.h>

#include <stdlib.h>
#include <string.h>

char* MDEStringDuplicate(const char* src){
	char* s = malloc(strlen(src) + 1);

	strcpy(s, src);

	return s;
}
