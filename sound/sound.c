#include <MDE/Sound/Sound.h>

#define TRY_DRIVER(name) \
	if(sound->context == NULL){ \
		sound->driver = MDESoundDriver ## name; \
		sound->context = MDESoundDriver ## name->open(path); \
	}

MDESound MDESoundOpen(const char* path){
	MDESound sound = malloc(sizeof(*sound));
	sound->context = NULL;

	TRY_DRIVER(MP3);

	if(sound->context == NULL){
		free(sound);
		return NULL;
	}

	if(sound->context->channels != 2 && sound->context->channels != 1){
		MDESoundClose(sound);
		return NULL;
	}

	return sound;
}

int MDESoundRead(MDESound sound, short* out, int frames){
	short* f = malloc(sizeof(*f) * sound->context->channels * frames);
	int r = sound->driver->read(sound->context, f, frames);
	int i;
	if(sound->context->channels == 2){
		for(i = 0; i < r * 2; i++){
			out[i] = f[i];
		}
	}else if(sound->context->channels == 1){
		for(i = 0; i < r; i++){
			out[i * 2 + 0] = f[i];
			out[i * 2 + 1] = f[i];
		}
		r *= 2;
	}
	free(f);

	return r;
}

int MDESoundReadFloat(MDESound sound, float* out, int frames){
	short* f = malloc(sizeof(*f) * 2 * frames);
	int i;
	int r = MDESoundRead(sound, f, frames);

	for(i = 0; i < r * 2; i++){
		out[i] = f[i] / 32767.0;
	}

	free(f);

	return r;
}

void MDESoundSeek(MDESound sound, int frames){
	sound->driver->seek(sound->context, frames);
}

void MDESoundClose(MDESound sound){
	sound->driver->close(sound->context);
	free(sound);
}
