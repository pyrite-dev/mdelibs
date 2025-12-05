#include <MDE/Core/AboutDialog.h>

#define Copyright "Copyright (C) 2025 Pyrite development team"

static void ok_activate(MwWidget handle, void* user, void* client) {
	MwDestroyWidget(user);
}

MwWidget MDEAboutDialog(MwWidget handle, const char* appname, const char* version, MwLLPixmap pixmap) {
	MwWidget    w, p, l, l2, l3, ok;
	MwSizeHints sh;
	int	    x = MwDEFAULT, y = MwDEFAULT;
	const int   iw = 128;
	const int   bh = 24, bw = bh * 3;
	int	    ww = 320, wh = 5 + iw + 5 + (16 + 5) * 3 + 10 + bh + 5;
	char	    buf[512];

	if(handle != NULL) {
		x = MwGetInteger(handle, MwNx) + (MwGetInteger(handle, MwNwidth) - ww) / 2;
		y = MwGetInteger(handle, MwNy) + (MwGetInteger(handle, MwNheight) - wh) / 2;
	}

	sh.min_width = sh.max_width = ww;
	sh.min_height = sh.max_height = wh;

	w = MwVaCreateWidget(MwWindowClass, "about", NULL, x, y, ww, wh,
			     MwNtitle, "About",
			     MwNsizeHints, &sh,
			     NULL);

	p = MwVaCreateWidget(MwImageClass, "pic", w, (ww - iw) / 2, 5, iw, iw,
			     MwNpixmap, pixmap,
			     NULL);

	l = MwVaCreateWidget(MwLabelClass, "label", w, 5, 5 + iw + 5, ww - 5 * 2, 16,
			     MwNtext, appname,
			     MwNbold, 1,
			     NULL);

	sprintf(buf, "Version %s", version);

	l2 = MwVaCreateWidget(MwLabelClass, "label2", w, 5, 5 + iw + 5 + 16 + 5, ww - 5 * 2, 16,
			      MwNtext, buf,
			      NULL);

	l3 = MwVaCreateWidget(MwLabelClass, "label3", w, 5, 5 + iw + 5 + 16 + 5 + 16 + 5 + 10, ww - 5 * 2, 16,
			      MwNtext, Copyright,
			      NULL);

	ok = MwVaCreateWidget(MwButtonClass, "ok", w, ww - 5 - bw, 5 + iw + 5 + 16 + 5 + 16 + 5 + 10 + 16 + 5, bw, bh,
			      MwNtext, "OK",
			      NULL);

	MwAddUserHandler(ok, MwNactivateHandler, ok_activate, w);

	if(handle != NULL) MwReparent(w, handle);

	return w;
}
