#ifndef __MDE_COMMANDLINE_H__
#define __MDE_COMMANDLINE_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _MDECommandLine MDECommandLine;

struct _MDECommandLine {
	const char* short_opt;
	const char* long_opt;
	int need_arg;
	void(*call)(const char* arg);
};

int MDECommandLineParse(MDECommandLine* param, int count, int argc, char** argv);

#ifdef __cplusplus
}
#endif

#endif
