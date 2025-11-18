#include <MDE/Directory.h>

#include <stdlib.h>
#include <string.h>
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
