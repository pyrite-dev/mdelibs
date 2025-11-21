#ifndef __MDE_AUDIO_H__
#define __MDE_AUDIO_H__

#include <MDE/MachDep.h>

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

/* always stereo, btw */
#define MDEAudioRate 48000

/* S16, stereo. frames*2*2 == bytes */
typedef void(*MDEAudioHandler)(void* handle, void* user, void* data, int frames);

/* driver functions */
void* MDEAudioOpen(MDEAudioHandler handler, void* user);

void MDEAudioClose(void* handle);

void MDEAudioStart(void* handle);

#ifdef __cplusplus
}
#endif

#endif
