#ifndef __MDE_CORE_ABOUTIALOG_H__
#define __MDE_CORE_ABOUTIALOG_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <Mw/Milsko.h>

MwWidget MDEAboutDialog(MwWidget handle, const char* appname, const char* version, MwLLPixmap pixmap);

#ifdef __cplusplus
}
#endif

#endif
