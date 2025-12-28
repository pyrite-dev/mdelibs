#ifndef __MDE_CORE_STRING_H__
#define __MDE_CORE_STRING_H__

#include <MDE/MachDep.h>

#ifdef __cplusplus
extern "C" {
#endif

char* MDEStringDuplicate(const char* src);

char* MDEStringConcatenate(const char* str1, const char* str2);

char* MDEStringConcatenate3(const char* str1, const char* str2, const char* str3);

char* MDEStringConcatenate4(const char* str1, const char* str2, const char* str3, const char* str4);

#ifdef __cplusplus
}
#endif

#endif
