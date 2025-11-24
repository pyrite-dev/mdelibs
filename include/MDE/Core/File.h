#ifndef __MDE_CORE_FILE_H__
#define __MDE_CORE_FILE_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

void MDEFileCopy(const char* src, const char* dst);

char* MDEFileAbsolutePath(const char* path);

char* MDEFileOptimizeAbsolutePath(const char* path);

#ifdef __cplusplus
}
#endif

#endif
