#include <MDE/Sound/Sound.h>
#include <MDE/Sound/ID3.h>

#define DR_MP3_IMPLEMENTATION
#include <dr_mp3.h>

static MDESoundContext mp3_open(const char* path) {
	MDESoundContext ctx = malloc(sizeof(*ctx));
	char*		s;
	memset(ctx, 0, sizeof(*ctx));

	ctx->title  = MDEID3GetString(path, "TIT2");
	ctx->artist = MDEID3GetString(path, "TPE1");
	ctx->album  = MDEID3GetString(path, "TALB");
	ctx->genre  = MDEID3GetString(path, "TCON");
	ctx->track  = 0;

	s = MDEID3GetString(path, "TRCK");
	if(s != NULL) {
		char* t = strchr(s, '/');
		if(t != NULL) t[0] = 0;

		ctx->track = atoi(s);

		free(s);
	}

	ctx->opaque = malloc(sizeof(drmp3));
	if(!drmp3_init_file(ctx->opaque, path, NULL)) {
		free(ctx);
		return NULL;
	}

	ctx->channels	 = ((drmp3*)ctx->opaque)->channels;
	ctx->sample_rate = ((drmp3*)ctx->opaque)->sampleRate;
	ctx->frames	 = drmp3_get_pcm_frame_count(ctx->opaque);

	return ctx;
}

static void mp3_close(MDESoundContext ctx) {
	drmp3_uninit(ctx->opaque);
	free(ctx->opaque);
	free(ctx);
}

static int mp3_read(MDESoundContext ctx, short* out, int frames) {
	return drmp3_read_pcm_frames_s16(ctx->opaque, frames, out);
}

static void mp3_seek(MDESoundContext ctx, int frames) {
	drmp3_seek_to_pcm_frame(ctx->opaque, frames);
}

static MDESoundDriverRec rec = {
    mp3_open,
    mp3_close,
    mp3_read,
    mp3_seek};
MDESoundDriver MDESoundDriverMP3 = &rec;
