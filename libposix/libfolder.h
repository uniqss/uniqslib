#pragma once

#if !defined(WIN32)&&!defined(WINDOWS)
#include <sys/stat.h>
#else
#error "not implemented"

#endif


int CreateDir(const char *sPathName);


