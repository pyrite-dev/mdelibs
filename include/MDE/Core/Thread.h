#ifndef __MDE_CORE_THREAD_H__
#define __MDE_CORE_THREAD_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* MDEThread;

MDEThread MDEThreadCreate(void (*call)(void* user_data), void* user_data);

void MDEThreadDestroy(MDEThread thread);

void MDEThreadJoin(MDEThread thread);

#ifdef __cplusplus
}
#endif

#endif
