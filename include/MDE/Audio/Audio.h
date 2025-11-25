#ifndef __MDE_AUDIO_AUDIO_H__
#define __MDE_AUDIO_AUDIO_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

/* always stereo, btw */
#define MDEAudioRate 48000

typedef void* MDEAudio;

/* S16, stereo. frames*2*2 == bytes */
typedef void (*MDEAudioHandler)(MDEAudio handle, void* user, void* data, int frames);

/* driver functions */
MDEAudio MDEAudioOpen(MDEAudioHandler handler, void* user);

void MDEAudioClose(MDEAudio handle);

void MDEAudioStart(MDEAudio handle);

#ifdef __cplusplus
}
#endif

#endif
