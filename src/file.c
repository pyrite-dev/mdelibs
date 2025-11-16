#include <MDE/File.h>

#include <stdio.h>

void MDEFileCopy(const char* src, const char* dst){
	char buffer[4096];
	FILE* in;
	FILE* out;
	int r;
	if((in = fopen(src, "rb")) == NULL) return;
	if((out = fopen(dst, "wb")) == NULL){
		fclose(in);
		return;
	}

	do{
		r = fread(buffer, 1, sizeof(buffer), in);
		fwrite(buffer, 1, r, out);
	}while(r == sizeof(buffer));

	fclose(in);
	fclose(out);
}
