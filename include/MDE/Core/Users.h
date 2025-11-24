#ifndef __MDE_CORE_USERS_H__
#define __MDE_CORE_USERS_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

void MDEUsersList(void (*call)(const char* name, void* user), void* user);

#ifdef __cplusplus
}
#endif

#endif
