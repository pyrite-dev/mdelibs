#include <MDE/Core/Users.h>
#include <MDE/Core/String.h>

#include <pwd.h>

void MDEUsersList(void (*call)(const char* name, void* user), void* user) {
	struct passwd* pwd;

	setpwent();
	while((pwd = getpwent()) != NULL) {
		char* dir;
		char* shell;

		/* this is BAD check. and i cannot do anything about it
		 *
		 * i appreciate the insanity
		 */

		if(pwd->pw_dir == NULL || pwd->pw_shell == NULL) continue;
		if(pwd->pw_name[0] == '_') continue;

		if(pwd->pw_uid != 0 && strstr(pwd->pw_dir, "/home/") == NULL) continue;

		shell = strrchr(pwd->pw_shell, '/');
		if(shell == NULL) break;
		shell++;

		if(strcmp(shell, "nologin") == 0) continue;

		dir = strrchr(pwd->pw_dir, '/');
		if(dir == NULL) break;
		dir++;

		if(strcmp(dir, pwd->pw_name) != 0) continue;

		call(pwd->pw_name, user);
	}
	endpwent();
}
