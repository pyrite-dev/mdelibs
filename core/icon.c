#include "config.h"

#include <MDE/Core/Icon.h>
#include <MDE/Core/String.h>

#include <ini.h>
#include <stb_ds.h>

typedef struct entry {
	char* key;
	int   value;
	char* genre;
	int   scalable;
} entry_t;

typedef struct opaque {
	entry_t* entries;
} opaque_t;

static int dumper(void* user, const char* section, const char* key, const char* value) {
	opaque_t* o = user;

	if(strcmp(section, "Icon Theme") != 0 && strcmp(key, "Size") == 0 && value != NULL) {
		int v = atoi(value);
		int ind;

		if((ind = shgeti(o->entries, section)) == -1) {
			shput(o->entries, section, 0);

			ind = shgeti(o->entries, section);

			o->entries[ind].genre	 = NULL;
			o->entries[ind].scalable = 0;
		}

		o->entries[ind].value = v;
	} else if(strcmp(section, "Icon Theme") != 0 && strcmp(key, "Context") == 0 && value != NULL) {
		int v = atoi(value);
		int ind;

		if((ind = shgeti(o->entries, section)) == -1) {
			shput(o->entries, section, 0);

			ind = shgeti(o->entries, section);

			o->entries[ind].genre	 = NULL;
			o->entries[ind].scalable = 0;
		}

		if(o->entries[ind].genre != NULL) free(o->entries[ind].genre);
		o->entries[ind].genre = MDEStringDuplicate(value);
	} else if(strcmp(section, "Icon Theme") != 0 && strcmp(key, "Type") == 0 && value != NULL && strcmp(value, "Scalable") == 0) {
		int ind;

		if((ind = shgeti(o->entries, section)) == -1) {
			shput(o->entries, section, 0);

			ind = shgeti(o->entries, section);

			o->entries[ind].genre	 = NULL;
			o->entries[ind].scalable = 0;
		}

		o->entries[ind].scalable = 1;
	}

	return 1;
}

static int sort_num(const void* a, const void* b) {
	return ((entry_t*)b)->value - ((entry_t*)a)->value;
}

static char* try_icon(const char* dir, const char* genre, const char* name, int size) {
	opaque_t o;
	char*	 ini = MDEStringConcatenate(dir, "/index.theme");
	char*	 p   = NULL;
	int	 i;

	o.entries = NULL;

	sh_new_strdup(o.entries);

	if(ini_parse(ini, dumper, &o) >= 0) {
		entry_t* a = NULL;

		for(i = 0; i < shlen(o.entries); i++) arrput(a, o.entries[i]);

		qsort(a, arrlen(a), sizeof(*a), sort_num);

		for(i = 0; i < arrlen(a); i++) {
			char* pth = NULL;
			int   c	  = (genre == NULL || (a[i].genre != NULL && strcmp(genre, a[i].genre) == 0)) ? 1 : 0;

			if(size == 0 && c) {
				pth = MDEStringConcatenate3(dir, "/", a[i].key);
			} else if(size == a[i].value && c) {
				pth = MDEStringConcatenate3(dir, "/", a[i].key);
			}

			if(pth != NULL) {
				char* f	 = MDEStringConcatenate4(pth, "/", name, ".png");
				FILE* fp = fopen(f, "rb");

				if(fp != NULL) {
					if(p != NULL) free(p);
					p = f;

					fclose(fp);

					free(pth);

					break;
				} else {
					free(f);
				}

				free(pth);
			}
		}

		arrfree(a);
	}

	for(i = 0; i < shlen(o.entries); i++) {
		if(o.entries[i].genre != NULL) free(o.entries[i].genre);
	}
	shfree(o.entries);

	return p;
}

char* MDEIconLookUp(const char* genre, const char* name, int size) {
	char* s;

	if((s = try_icon(DATAROOTDIR "/icons/mde", genre, name, size)) != NULL) return s;
	if((s = try_icon(DATAROOTDIR "/icons/hicolor", genre, name, size)) != NULL) return s;
#ifdef __NetBSD__
	if((s = try_icon("/usr/pkg/share/icons/hicolor", genre, name, size)) != NULL) return s;
#endif
	if((s = try_icon("/usr/share/icons/hicolor", genre, name, size)) != NULL) return s;

	return NULL;
}
