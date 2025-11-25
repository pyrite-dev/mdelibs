#ifndef __MDE_CORE_MUTEX_H__
#define __MDE_CORE_MUTEX_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* MDEMutex;

MDEMutex MDEMutexCreate(void);

void MDEMutexDestroy(MDEMutex mutex);

void MDEMutexLock(MDEMutex mutex);

void MDEMutexUnlock(MDEMutex mutex);

#ifdef __cplusplus
}
#endif

#endif
