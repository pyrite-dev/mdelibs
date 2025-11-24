static void* thread_routine(void* arg) {
	driver_t* drv = arg;
	void*	  data;

	data = malloc(drv->frames * 2 * 2);

	while(1) {
		drv->handler(drv, drv->user, data, drv->frames);

		drv_write(drv, data, drv->frames);

		pthread_mutex_lock(&drv->mutex);
		if(drv->quit) {
			pthread_mutex_unlock(&drv->mutex);
			break;
		}
		pthread_mutex_unlock(&drv->mutex);
	}

	free(data);

	return NULL;
}

MDEAudio MDEAudioOpen(MDEAudioHandler handler, void* user) {
	int	  rate = MDEAudioRate;
	driver_t* drv  = malloc(sizeof(*drv));
	memset(drv, 0, sizeof(*drv));

	drv->quit    = 0;
	drv->handler = handler;
	drv->user    = user;
	drv->init    = 0;

	pthread_mutex_init(&drv->mutex, NULL);

	if(!(drv->init = drv_open(drv))) {
		MDEAudioClose(drv);
		return NULL;
	}

	return drv;
}

void MDEAudioClose(MDEAudio handle) {
	driver_t* drv = handle;
	void*	  ret;

	if(drv->init) {
		pthread_mutex_lock(&drv->mutex);
		drv->quit = 1;
		pthread_mutex_unlock(&drv->mutex);
		pthread_join(drv->thread, &ret);
	}

	pthread_mutex_destroy(&drv->mutex);

	drv_close(drv);

	free(drv);
}

void MDEAudioStart(MDEAudio handle) {
	driver_t* drv = handle;

	pthread_create(&drv->thread, NULL, thread_routine, handle);
}
