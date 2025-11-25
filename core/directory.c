#include <MDE/Core/Directory.h>
#include <MDE/Core/String.h>

#include <Mw/Milsko.h>
#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#else
#include <dirent.h>
#endif
#include <sys/stat.h>

void MDEDirectoryScan(const char* path, void (*call)(const char* name, int dir, int symlink, void* user), void* user) {
	void* dir = MwDirectoryOpen(path);
	if(dir != NULL) {
		MwDirectoryEntry* d;
		while((d = MwDirectoryRead(dir)) != NULL) {
			char*	    p;
			struct stat s;
			if(strcmp(d->name, "..") == 0 || strcmp(d->name, ".") == 0) continue;

			p = malloc(strlen(path) + 1 + strlen(d->name) + 1);
			strcpy(p, path);
#ifdef _WIN32
			if(path[strlen(path) - 1] != '\\') strcat(p, "\\");
#else
			if(path[strlen(path) - 1] != '/') strcat(p, "/");
#endif
			strcat(p, d->name);

#ifdef _WIN32
			if(stat(p, &s) == 0) {
				call(p, d->type == MwDIRECTORY_DIRECTORY ? 1 : 0, 0, user);
			}
#else
			if(lstat(p, &s) == 0) {
				call(p, S_ISDIR(s.st_mode) ? 1 : 0, S_ISLNK(s.st_mode) ? 1 : 0, user);
			}
#endif

			free(p);
		}
		MwDirectoryClose(dir);
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
#ifdef _WIN32
		if(path[i] == '\\' || path[i] == 0) {
			mkdir(r);
#else
		if(path[i] == '/' || path[i] == 0) {
			mkdir(r, mode);
#endif
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

char* MDEDirectoryConfigPath(void) {
#ifdef _WIN32
	char*	     shp = malloc(MAX_PATH);
	LPITEMIDLIST pidl;
	if(SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &pidl) == S_OK) {
		SHGetPathFromIDList(pidl, shp);
		CoTaskMemFree(pidl);
		return shp;
	}
	free(shp);

	shp = getenv("USERPROFILE");
	return shp == NULL ? NULL : MDEStringDuplicate(shp);
#else
	struct passwd* pwd = getpwuid(getuid());
	char*	       p   = malloc(strlen(pwd->pw_dir) + strlen("/.config") + 1);
	strcpy(p, pwd->pw_dir);
	strcat(p, "/.config");

	return p;
#endif
}
