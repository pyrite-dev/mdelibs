#include <MDE/Core/Mutex.h>

#ifdef _WIN32
#include <windows.h>

MDEMutex MDEMutexCreate(void) {
	return CreateEvent(NULL, FALSE, TRUE, NULL);
}

void MDEMutexDestroy(MDEMutex mutex) {
	CloseHandle(mutex);
}

void MDEMutexLock(MDEMutex mutex) {
	WaitForSingleObject(mutex, INFINITE);
}

void MDEMutexUnlock(MDEMutex mutex) {
	SetEvent(mutex);
}
#else
#endif
