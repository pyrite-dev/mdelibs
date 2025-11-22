#include <MDE/Audio/Audio.h>

#include <pthread.h>

typedef struct driver {
	driver_context_t context;

	int init;
	int frames;

	MDEAudioHandler handler;
	void* user;

	int quit;
	pthread_t thread;
	pthread_mutex_t mutex;
} driver_t;
