#include <MDE/Core/Thread.h>

#ifdef _WIN32
#include <windows.h>

static DWORD WINAPI call_wrapper(LPVOID param) {
	void** p		      = param;
	void (*call)(void* user_data) = p[0];
	void* ud		      = p[1];
	free(p);

	call(ud);

	return 0;
}

MDEThread MDEThreadCreate(void (*call)(void* user_data), void* user_data) {
	void** p = malloc(sizeof(*p) * 2);

	p[0] = call;
	p[1] = user_data;

	return CreateThread(NULL, 0, call_wrapper, (void*)p, 0, NULL);
}

void MDEThreadDestroy(MDEThread thread) {
	CloseHandle(thread);
}

void MDEThreadJoin(MDEThread thread) {
	WaitForSingleObject(thread, INFINITE);
}
#else
#include <pthread.h>

static void* call_wrapper(void* param) {
	void** p		      = param;
	void (*call)(void* user_data) = p[0];
	void* ud		      = p[1];
	free(p);

	call(ud);

	return NULL;
}

MDEThread MDEThreadCreate(void (*call)(void* user_data), void* user_data) {
	void**	   p = malloc(sizeof(*p) * 2);
	pthread_t* t = malloc(sizeof(*t));

	p[0] = call;
	p[1] = user_data;

	pthread_create(t, NULL, call_wrapper, p);
	return t;
}

void MDEThreadDestroy(MDEThread thread) {
	free(thread);
}

void MDEThreadJoin(MDEThread thread) {
	void* p;
	pthread_join(thread, &p);
}
#endif
