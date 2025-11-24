#include <MDE/Sound/ID3.h>
#include <MDE/Core/String.h>

static unsigned int syncsafe(FILE* f) {
        unsigned char d[4];
        unsigned int  r = 0;
        int           i;
        fread(d, 4, 1, f);

        for(i = 0; i < 4; i++) {
                r = r << 7;
                r = r | (d[i] & 127);
        }

        return r;
}

static unsigned int big(FILE* f) {
        unsigned char d[4];
        unsigned int  r = 0;
        int           i;
        fread(d, 4, 1, f);

        for(i = 0; i < 4; i++) {
                r = r << 8;
                r = r | d[i];
        }

        return r;
}

char* MDEID3GetTag(const char* path, const char* name, size_t* size){
        FILE*          f = fopen(path, "rb");
        unsigned char* d = NULL;
        char           sig[3];
        unsigned char  ver[2];
        unsigned char  flag;
        unsigned char  frame[4];
        unsigned int   dsz = 0;
        if(f == NULL) return NULL;

        fread(sig, 3, 1, f);
        if(memcmp(sig, "ID3", 3) != 0) return NULL;

        fread(ver, 2, 1, f);
        if(memcmp(ver, "\x04\x00", 2) != 0 && memcmp(ver, "\x03\x00", 2) != 0) return NULL;

        fread(&flag, 1, 1, f);
        if(flag & (1 << 6)) return NULL; /* FIXME */
        if(flag & (1 << 7)) return NULL; /* what even is this??? */

        dsz = syncsafe(f);

        while(dsz > 0) {
                unsigned int fsz;

                fread(frame, 4, 1, f);
                if(frame[0] == 0) break; /* XXX: Is this ok? */

                if(memcmp(ver, "\x04\x00", 2) == 0) {
                        fsz = syncsafe(f);
                } else {
                        fsz = big(f);
                }

                fseek(f, 2, SEEK_CUR);
                if(memcmp(frame, name, strlen(name)) == 0) {
			d = malloc(fsz);
			fread(d, fsz, 1, f);

			*size = fsz;
			break;
                } else {
                        fseek(f, fsz, SEEK_CUR);
                }

                dsz -= 4 + 4 + 2 + fsz;
        }

        fclose(f);

        return d;
}

char* MDEID3GetString(const char* path, const char* name){
	size_t sz;
	unsigned char* d = MDEID3GetTag(path, name, &sz);
	char* s;
	if(d == NULL) return NULL;
	if(d[0] == 1 || d[0] == 2){
		free(d);
		return NULL;
	}

	s = malloc(sz - 1 + 1);
	memcpy(s, d + 1, sz - 1);
	s[sz - 1] = 0;

	free(d);

	return s;
}
