#include <MDE/Audio/Audio.h>

#ifdef USE_AO
#include <ao/ao.h>

typedef struct driver_context {
	ao_sample_format format;
	ao_device*	 device;
} driver_context_t;

#include "thread/decl.c"

static void drv_write(driver_t* drv, void* data, int frames) {
	ao_play(drv->context.device, (char*)data, frames * 2 * 2);
}

static int drv_open(driver_t* drv) {
	int id;

	ao_initialize();

	drv->context.format.bits	= 16;
	drv->context.format.rate	= MDEAudioRate;
	drv->context.format.channels	= 2;
	drv->context.format.byte_format = AO_FMT_NATIVE;
	drv->context.format.matrix	= NULL;

	id = ao_default_driver_id();
	if((drv->context.device = ao_open_live(id, &drv->context.format, NULL)) == NULL) {
		MDEAudioClose(drv);
		return 0;
	}

	drv->frames = 80 * MDEAudioRate / 1000;

	return 1;
}

static void drv_close(driver_t* drv) {
	if(drv->context.device != NULL) {
		ao_close(drv->context.device);
	}
}

#include "thread/body.c"
#endif
