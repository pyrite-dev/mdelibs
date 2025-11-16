#ifndef __MDE_DIRECTORY_H__
#define __MDE_DIRECTORY_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

void MDEDirectoryScan(const char* path, void(*call)(const char* name, int dir, void* user), void* user);

#ifdef __cplusplus
}
#endif

#endif
