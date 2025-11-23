#include <MDE/Core/Directory.h>
#include <MDE/Core/String.h>

#include <dirent.h>
#include <sys/stat.h>

void MDEDirectoryScan(const char* path, void (*call)(const char* name, int dir, int symlink, void* user), void* user) {
	DIR* dir = opendir(path);
	if(dir != NULL) {
		struct dirent* d;
		while((d = readdir(dir)) != NULL) {
			char*	    p;
			struct stat s;
			if(strcmp(d->d_name, "..") == 0 || strcmp(d->d_name, ".") == 0) continue;

			p = malloc(strlen(path) + 1 + strlen(d->d_name) + 1);
			strcpy(p, path);
			if(path[strlen(path) - 1] != '/') strcat(p, "/");
			strcat(p, d->d_name);

			if(lstat(p, &s) == 0) {
				call(p, S_ISDIR(s.st_mode) ? 1 : 0, S_ISLNK(s.st_mode) ? 1 : 0, user);
			}

			free(p);
		}
		closedir(dir);
	}
}

void MDEDirectoryCreate(const char* path, int mode) {
	int   i;
	char* r = malloc(strlen(path) + 1);
	r[0]	= 0;
	for(i = 0;; i++) {
		char b[2];
		b[0] = path[i];
		b[1] = 0;
		strcat(r, b);
		if(path[i] == '/' || path[i] == 0) {
			mkdir(r, mode);
			if(path[i] == 0) break;
		}
	}
	free(r);
}

char* MDEDirectoryCurrentPath(void) {
	char p[4097]; /* your system is crazy if it allows more than 4096 letters */
	getcwd(p, 4096);

	return MDEStringDuplicate(p);
}
