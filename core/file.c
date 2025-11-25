#include <MDE/Core/File.h>
#include <MDE/Core/String.h>
#include <MDE/Core/Directory.h>

#ifdef _WIN32
#include <windows.h>
#endif

void MDEFileCopy(const char* src, const char* dst) {
	char  buffer[4096];
	FILE* in;
	FILE* out;
	int   r;
	if((in = fopen(src, "rb")) == NULL) return;
	if((out = fopen(dst, "wb")) == NULL) {
		fclose(in);
		return;
	}

	do {
		r = fread(buffer, 1, sizeof(buffer), in);
		fwrite(buffer, 1, r, out);
	} while(r == sizeof(buffer));

	fclose(in);
	fclose(out);
}

char* MDEFileOptimizeAbsolutePath(const char* path) {
	char* r	  = MDEStringDuplicate(path);
	char* buf = MDEStringDuplicate(path);
	int   i;
	r[0] = buf[0] = 0;

	for(i = 0;; i++) {
		if(path[i] == '/' || path[i] == 0) {
			if(strcmp(buf, ".") == 0) {
			} else if(strcmp(buf, "..") == 0) {
				char* p;

				if((p = strrchr(r, '/')) != NULL) p[0] = 0;
				if((p = strrchr(r, '/')) != NULL) {
					p[0] = 0;
					strcat(r, "/");
				}
				if(p == NULL) {
					strcat(r, "/");
				}
			} else {
				strcat(r, buf);
				if(path[i] == '/') strcat(r, "/");
			}

			buf[0] = 0;

			if(path[i] == 0) break;
		} else {
			char cbuf[2];
			cbuf[0] = path[i];
			cbuf[1] = 0;

			strcat(buf, cbuf);
		}
	}

	free(buf);

	return r;
}

char* MDEFileAbsolutePath(const char* path) {
#ifdef _WIN32
	char* r = malloc(MAX_PATH);

	GetFullPathName(path, MAX_PATH, r, NULL);

	return r;
#else
	char* p;
	char* r;

	if(path[0] == '/') return MDEStringDuplicate(path);

	p = MDEDirectoryCurrentPath();
	r = malloc(strlen(p) + 1 + strlen(path) + 1);
	strcpy(r, p);
	if(p[strlen(p) - 1] != '/') strcat(r, "/");
	strcat(r, path);

	free(p);

	p = r;
	r = MDEFileOptimizeAbsolutePath(p);
	free(p);

	return r;
#endif
}
