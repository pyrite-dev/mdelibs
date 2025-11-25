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
#include <pthread.h>

MDEMutex MDEMutexCreate(void) {
	pthread_mutex_t* m = malloc(sizeof(*m));
	pthread_mutex_init(m, NULL);
	return m;
}

void MDEMutexDestroy(MDEMutex mutex) {
	pthread_mutex_destroy(mutex);
	free(mutex);
}

void MDEMutexLock(MDEMutex mutex) {
	pthread_mutex_lock(mutex);
}

void MDEMutexUnlock(MDEMutex mutex) {
	pthread_mutex_unlock(mutex);
}
#endif
