#include <MDE/Audio/Audio.h>

#include <pthread.h>

#include <alsa/asoundlib.h>

typedef struct driver {
	MDEAudioHandler handler;
	void* user;
	snd_pcm_t* device;
	snd_pcm_hw_params_t* params;

	int quit;
	pthread_t thread;
	pthread_mutex_t mutex;
} driver_t;

static void* thread_routine(void* arg){
	driver_t* drv = arg;
	snd_pcm_uframes_t frame;
	void* data;

	snd_pcm_hw_params_get_period_size(drv->params, &frame, 0);

	data = malloc(frame * 2 * 2);

	while(1){
		drv->handler(drv, drv->user, data, frame);
		if(snd_pcm_writei(drv->device, data, frame) == -EPIPE){
			snd_pcm_prepare(drv->device);
		}

		pthread_mutex_lock(&drv->mutex);
		if(drv->quit){
			pthread_mutex_unlock(&drv->mutex);
			break;
		}
		pthread_mutex_unlock(&drv->mutex);
	}

	free(data);

	return NULL;
}

void* MDEAudioOpen(MDEAudioHandler handler, void* user){
	int rate = MDEAudioRate;
	driver_t* drv = malloc(sizeof(*drv));
	memset(drv, 0, sizeof(*drv));

	drv->quit = 0;
	drv->handler = handler;
	drv->user = user;

	if(snd_pcm_open(&drv->device, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0){
		MDEAudioClose(drv);
		return NULL;
	}

	snd_pcm_hw_params_malloc(&drv->params);
	snd_pcm_hw_params_any(drv->device, drv->params);
	
	if(snd_pcm_hw_params_set_access(drv->device, drv->params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0){
		MDEAudioClose(drv);
		return NULL;
	}
	
	if(snd_pcm_hw_params_set_format(drv->device, drv->params, SND_PCM_FORMAT_S16) < 0){
		MDEAudioClose(drv);
		return NULL;
	}

	if(snd_pcm_hw_params_set_channels(drv->device, drv->params, 2) < 0){
		MDEAudioClose(drv);
		return NULL;
	}

	if(snd_pcm_hw_params_set_rate_near(drv->device, drv->params, &rate, 0) < 0){
		MDEAudioClose(drv);
		return NULL;
	}

	if(snd_pcm_hw_params_set_rate(drv->device, drv->params, rate, 0) < 0){
		MDEAudioClose(drv);
		return NULL;
	}

	if(snd_pcm_hw_params(drv->device, drv->params) < 0){
		MDEAudioClose(drv);
		return NULL;
	}

	pthread_mutex_init(&drv->mutex, NULL);

	return drv;
}

void MDEAudioClose(void* handle){
	driver_t* drv = handle;

	if(drv->device != NULL){
		void* ret;

		pthread_mutex_lock(&drv->mutex);
		drv->quit = 1;
		pthread_mutex_unlock(&drv->mutex);
		pthread_join(drv->thread, &ret);

		pthread_mutex_destroy(&drv->mutex);

		snd_pcm_drain(drv->device);
		snd_pcm_close(drv->device);
		snd_pcm_hw_params_free(drv->params);
	}

	free(drv);
}

void MDEAudioStart(void* handle){
	driver_t* drv = handle;

	pthread_create(&drv->thread, NULL, thread_routine, handle);
}
