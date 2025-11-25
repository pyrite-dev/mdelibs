#include <MDE/Audio/Audio.h>
#include <MDE/Core/Thread.h>
#include <MDE/Core/Mutex.h>

typedef struct driver {
	driver_context_t context;

	int init;
	int frames;

	MDEAudioHandler handler;
	void*		user;

	int	  quit;
	MDEThread thread;
	MDEMutex  mutex;
} driver_t;
