#include <MDE/Sound/Sound.h>
#include <MDE/Core/String.h>

#define DR_FLAC_IMPLEMENTATION
#include <dr_flac.h>

static void meta(void* user, drflac_metadata* metadata) {
	MDESoundContext ctx = user;

	if(metadata->type == DRFLAC_METADATA_BLOCK_TYPE_VORBIS_COMMENT) {
		const char*		       comment;
		drflac_vorbis_comment_iterator it;
		drflac_uint32		       len;

		drflac_init_vorbis_comment_iterator(&it, metadata->data.vorbis_comment.commentCount, metadata->data.vorbis_comment.pComments);
		while((comment = drflac_next_vorbis_comment(&it, &len)) != NULL) {
			char* s = malloc(len + 1);
			char* p;
			memcpy(s, comment, len);
			s[len] = 0;
			p      = strchr(s, '=');
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
				} else if(strcasecmp(s, "TRACKNUMBER") == 0) {
					ctx->track = atoi(p + 1);
				}
			}

			free(s);
		}
	}
}

static MDESoundContext flac_open(const char* path) {
	MDESoundContext ctx = malloc(sizeof(*ctx));
	memset(ctx, 0, sizeof(*ctx));

	ctx->opaque = drflac_open_file_with_metadata(path, meta, ctx, NULL);
	if(ctx->opaque == NULL) {
		free(ctx);
		return NULL;
	}

	ctx->sample_rate = ((drflac*)ctx->opaque)->sampleRate;
	ctx->channels	 = ((drflac*)ctx->opaque)->channels;
	ctx->frames	 = ((drflac*)ctx->opaque)->totalPCMFrameCount;

	return ctx;
}

static void flac_close(MDESoundContext ctx) {
	drflac_close(ctx->opaque);
	free(ctx);
}

static int flac_read(MDESoundContext ctx, short* out, int frames) {
	return drflac_read_pcm_frames_s16(ctx->opaque, frames, out);
}

static void flac_seek(MDESoundContext ctx, int frames) {
	drflac_seek_to_pcm_frame(ctx->opaque, frames);
}

static MDESoundDriverRec rec = {
    flac_open,
    flac_close,
    flac_read,
    flac_seek};
MDESoundDriver MDESoundDriverFLAC = &rec;
