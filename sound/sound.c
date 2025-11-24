#include <MDE/Sound/Sound.h>

#include <stb_ds.h>

#define TRY_DRIVER(name) \
	if(sound->context == NULL){ \
		sound->driver = MDESoundDriver ## name; \
		sound->context = MDESoundDriver ## name->open(path); \
	}

MDESound MDESoundOpen(const char* path){
	MDESound sound = malloc(sizeof(*sound));
	sound->context = NULL;
	sound->buffer = NULL;

	TRY_DRIVER(MP3);
	TRY_DRIVER(Vorbis);

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
	int total = frames;
	int seek = 0;

	while(arrlen(sound->buffer) > 0 && total > 0){
		int fr = (sound->buffer[0].frames - sound->buffer[0].seek);
		if(fr > total) fr = total;

		memcpy(out + seek * 2, sound->buffer[0].data + sound->buffer[0].seek * 2, 2 * 2 * fr);

		sound->buffer[0].seek += fr;
		if(sound->buffer[0].seek == sound->buffer[0].frames){
			free(sound->buffer[0].data);
			arrdel(sound->buffer, 0);
		}
		total -= fr;
		seek += fr;
	}

	while(total > 0){
		int i;
		short* buf = malloc(sizeof(*buf) * 2 * frames);
		short* f = malloc(sizeof(*f) * sound->context->channels * frames);
		int r = sound->driver->read(sound->context, f, frames);
		if(sound->context->channels == 2){
			for(i = 0; i < r * 2; i++){
				buf[i] = f[i];
			}
		}else if(sound->context->channels == 1){
			for(i = 0; i < r; i++){
				buf[i * 2 + 0] = f[i];
				buf[i * 2 + 1] = f[i];
			}
		}
		free(f);

		if(r == 0){
			free(buf);
			break;
		}else if(total >= r){
			memcpy(out + seek * 2, buf, r * 2 * 2);
			seek += r;
			total -= r;
			free(buf);
		}else if(total < r){
			MDESoundBuffer buffer;
			buffer.data = buf;
			buffer.seek = total;
			buffer.frames = r;
			arrput(sound->buffer, buffer);

			memcpy(out + seek * 2, buf, total * 2 * 2);
			seek += total;
			total -= total;
		}
	}

	return seek;
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
	int i;
	if(sound->context->title != NULL) free(sound->context->title);
	if(sound->context->artist != NULL) free(sound->context->artist);
	if(sound->context->album != NULL) free(sound->context->album);
	if(sound->context->genre != NULL) free(sound->context->genre);

	sound->driver->close(sound->context);

	for(i = 0; i < arrlen(sound->buffer); i++){
		free(sound->buffer[i].data);
	}
	arrfree(sound->buffer);
	free(sound);
}
