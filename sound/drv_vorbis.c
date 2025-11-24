#include <MDE/Sound/Sound.h>
#include <MDE/Core/String.h>

#include <vorbis/vorbisfile.h>

static MDESoundContext vorbis_open(const char* path) {
	MDESoundContext ctx;
	FILE*		f = fopen(path, "rb");
	vorbis_comment* comments;
	if(f == NULL) return NULL;

	ctx = malloc(sizeof(*ctx));
	memset(ctx, 0, sizeof(*ctx));

	ctx->opaque = malloc(sizeof(OggVorbis_File));
	if(ov_open_callbacks(f, ctx->opaque, NULL, 0, OV_CALLBACKS_DEFAULT) < 0) {
		free(ctx->opaque);
		free(ctx);
		fclose(f);
		return NULL;
	}

	comments = ov_comment(ctx->opaque, -1);
	if(comments != NULL) {
		int i;
		for(i = 0; i < comments->comments; i++) {
			char* s = MDEStringDuplicate(comments->user_comments[i]);
			char* p = strchr(s, '=');
			if(p != NULL) {
				p[0] = 0;
				if(strcasecmp(s, "TITLE") == 0) {
					ctx->title = MDEStringDuplicate(p + 1);
				} else if(strcasecmp(s, "ALBUM") == 0) {
					ctx->album = MDEStringDuplicate(p + 1);
				} else if(strcasecmp(s, "ARTIST") == 0) {
					ctx->artist = MDEStringDuplicate(p + 1);
				} else if(strcasecmp(s, "GENRE") == 0) {
					ctx->genre = MDEStringDuplicate(p + 1);
				} else if(strcasecmp(s, "TRACK") == 0) {
					ctx->track = atoi(p + 1);
				}
			}

			free(s);
		}
	}

	ctx->channels	 = ov_info(ctx->opaque, -1)->channels;
	ctx->sample_rate = ov_info(ctx->opaque, -1)->rate;
	ctx->frames	 = ov_pcm_total(ctx->opaque, -1);

	return ctx;
}

static void vorbis_close(MDESoundContext ctx) {
	ov_clear(ctx->opaque);
	free(ctx->opaque);
	free(ctx);
}

static int vorbis_read(MDESoundContext ctx, short* out, int frames) {
	int n;
	return ov_read(ctx->opaque, (char*)out, frames * 2 * ctx->channels, 0, 2, 1, &n) / 2 / ctx->channels;
}

static void vorbis_seek(MDESoundContext ctx, int frames) {
	ov_pcm_seek(ctx->opaque, frames);
}

static MDESoundDriverRec rec = {
    vorbis_open,
    vorbis_close,
    vorbis_read,
    vorbis_seek};
MDESoundDriver MDESoundDriverVorbis = &rec;
