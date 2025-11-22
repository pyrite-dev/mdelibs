#include <MDE/Audio/Audio.h>

#ifdef USE_ALSA
#include <alsa/asoundlib.h>

typedef struct driver_context {
	snd_pcm_t* device;
	snd_pcm_hw_params_t* params;
} driver_context_t;

#include "pthread/decl.c"

static void drv_write(driver_t* drv, void* data, int frames){
	if(snd_pcm_writei(drv->context.device, data, frames) == -EPIPE){
		snd_pcm_prepare(drv->context.device);
	}
}

static int drv_open(driver_t* drv){
	snd_pcm_uframes_t frames;
	int rate = MDEAudioRate;
	
	if(snd_pcm_open(&drv->context.device, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0){
		MDEAudioClose(drv);
		return 0;
	}

	snd_pcm_hw_params_malloc(&drv->context.params);
	snd_pcm_hw_params_any(drv->context.device, drv->context.params);
	
	if(snd_pcm_hw_params_set_access(drv->context.device, drv->context.params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0){
		MDEAudioClose(drv);
		return 0;
	}
	
	if(snd_pcm_hw_params_set_format(drv->context.device, drv->context.params, SND_PCM_FORMAT_S16) < 0){
		MDEAudioClose(drv);
		return 0;
	}

	if(snd_pcm_hw_params_set_channels(drv->context.device, drv->context.params, 2) < 0){
		MDEAudioClose(drv);
		return 0;
	}

	if(snd_pcm_hw_params_set_rate_near(drv->context.device, drv->context.params, &rate, 0) < 0){
		MDEAudioClose(drv);
		return 0;
	}

	if(snd_pcm_hw_params_set_rate(drv->context.device, drv->context.params, rate, 0) < 0){
		MDEAudioClose(drv);
		return 0;
	}

	if(snd_pcm_hw_params(drv->context.device, drv->context.params) < 0){
		MDEAudioClose(drv);
		return 0;
	}

	snd_pcm_hw_params_get_period_size(drv->context.params, &frames, 0);
	drv->frames = frames;

	return 1;
}


static void drv_close(driver_t* drv){
	if(drv->context.device != NULL){
		snd_pcm_drain(drv->context.device);
		snd_pcm_close(drv->context.device);
		snd_pcm_hw_params_free(drv->context.params);
	}
}

#include "pthread/body.c"
#endif
