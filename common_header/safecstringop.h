#pragma once

#include "platform.h"

template<size_t _Size>
inline void safestrcpy(char(&_Dest)[_Size], const char * _Source, size_t _Count)
{
	size_t __SizeMin = _Size > _Count ? _Count : _Size;
#ifdef UNIQS_PLATFORM_WINDOWS
	strncpy_s(_Dest, _Source, __SizeMin);
#else
	strncpy(_Dest, _Source, __SizeMin);
#endif
}


#ifdef UNIQS_PLATFORM_WINDOWS
#define snprintf sprintf_s
#else
#define sprintf_s snprintf
#endif


