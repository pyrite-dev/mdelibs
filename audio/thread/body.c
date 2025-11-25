static void thread_routine(void* arg) {
	driver_t* drv = arg;
	void*	  data;

	data = malloc(drv->frames * 2 * 2);

	while(1) {
		drv->handler(drv, drv->user, data, drv->frames);

		drv_write(drv, data, drv->frames);

		MDEMutexLock(drv->mutex);
		if(drv->quit) {
			MDEMutexUnlock(&drv->mutex);
			break;
		}
		MDEMutexUnlock(drv->mutex);
	}

	free(data);
}

MDEAudio MDEAudioOpen(MDEAudioHandler handler, void* user) {
	int	  rate = MDEAudioRate;
	driver_t* drv  = malloc(sizeof(*drv));
	memset(drv, 0, sizeof(*drv));

	drv->quit    = 0;
	drv->handler = handler;
	drv->user    = user;
	drv->init    = 0;

	drv->mutex = MDEMutexCreate();

	if(!(drv->init = drv_open(drv))) {
		MDEAudioClose(drv);
		return NULL;
	}

	return drv;
}

void MDEAudioClose(MDEAudio handle) {
	driver_t* drv = handle;

	if(drv->init) {
		MDEMutexLock(drv->mutex);
		drv->quit = 1;
		MDEMutexLock(drv->mutex);
		MDEThreadJoin(drv->thread);
	}

	MDEMutexDestroy(drv->mutex);

	drv_close(drv);

	free(drv);
}

void MDEAudioStart(MDEAudio handle) {
	driver_t* drv = handle;

	drv->thread = MDEThreadCreate(thread_routine, handle);
}
