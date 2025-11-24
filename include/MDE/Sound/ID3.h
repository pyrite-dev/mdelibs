#ifndef __MDE_SOUND_ID3_H__
#define __MDE_SOUND_ID3_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

char* MDEID3GetTag(const char* path, const char* name, size_t* size);

char* MDEID3GetString(const char* path, const char* name);

#ifdef __cplusplus
}
#endif

#endif
