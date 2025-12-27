#include "config.h"

#include <MDE/Core/Icon.h>
#include <MDE/Core/String.h>

static char* try_file(const char* path, const char* genre, const char* name, int size) {
	char  sz[512];
	char* s;
	char* s2;
	char* s3;
	char* s4;
	char* s5;
	char* s6;
	char* t;
	FILE* f;

	sprintf(sz, "%dx%d", size, size);

	s  = MDEStringConcatenate(path, "/");
	s2 = MDEStringConcatenate(s, sz);
	s3 = MDEStringConcatenate(s2, "/");
	s4 = MDEStringConcatenate(s3, genre);
	s5 = MDEStringConcatenate(s4, "/");
	s6 = MDEStringConcatenate(s5, name);
	t  = MDEStringConcatenate(s6, ".png");

	free(s6);
	free(s5);
	free(s4);
	free(s3);
	free(s2);
	free(s);

	if((f = fopen(t, "rb")) == NULL) {
		free(t);
		return NULL;
	}
	fclose(f);

	return t;
}

char* MDEIconLookUp(const char* genre, const char* name, int size) {
	char*	    s;
	const char* g	     = genre;
	int	    i	     = 0;
	const char* genres[] = {
	    "actions",
	    "apps",
	    "categories",
	    "devices",
	    "emblems",
	    "emotes",
	    "mimetypes",
	    "places",
	    "status",
	    NULL};

	do {
		if(genre == NULL) g = genres[i];
		if(g == NULL) break;
		if((s = try_file(DATAROOTDIR "/icons/mde", g, name, size)) != NULL) return s;
		if((s = try_file(DATAROOTDIR "/icons/hicolor", g, name, size)) != NULL) return s;
#ifdef __NetBSD__
		if((s = try_file("/usr/pkg/share/icons/hicolor", g, name, size)) != NULL) return s;
#endif
		if((s = try_file("/usr/share/icons/hicolor", g, name, size)) != NULL) return s;

		i++;

		if(genre != NULL) break;
	} while(1);

	return NULL;
}
