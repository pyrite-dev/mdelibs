#ifndef __MDE_SOUND_SOUND_H__
#define __MDE_SOUND_SOUND_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MDESoundDriver	 MDESoundDriverRec, *MDESoundDriver;
typedef struct _MDESoundContext* MDESoundContext;
typedef struct _MDESound*	 MDESound;
typedef struct _MDESoundBuffer	 MDESoundBuffer;

struct _MDESoundContext {
	int   sample_rate;
	int   channels;
	int   frames;
	void* opaque;

	char* title;
	char* album;
	char* artist;
	char* genre;
};

struct _MDESoundDriver {
	MDESoundContext (*open)(const char* path);
	void (*close)(MDESoundContext context);
	int (*read)(MDESoundContext context, short* out, int frames);
	void (*seek)(MDESoundContext context, int frames);
};

struct _MDESoundBuffer {
	short* data;
	int    frames;
	int    seek;
};

struct _MDESound {
	MDESoundContext context;
	MDESoundDriver	driver;
	MDESoundBuffer* buffer;
};

extern MDESoundDriver MDESoundDriverMP3;
extern MDESoundDriver MDESoundDriverVorbis;

MDESound MDESoundOpen(const char* path);

int MDESoundRead(MDESound sound, short* out, int frames);

int MDESoundReadFloat(MDESound sound, float* out, int frames);

void MDESoundSeek(MDESound sound, int frames);

void MDESoundClose(MDESound sound);

#ifdef __cplusplus
}
#endif

#endif
