#ifndef PUMP_TYPES_H_
#define PUMP_TYPES_H_
#include <cstdint>
#include <pump/config/Platform.h>

namespace pump
{
    typedef unsigned char BYTE_T;
	
#ifdef PUMP_PLATFORM_WIN
    typedef void* SOCKET;
#else
    typedef int SOCKET;
#endif
}

#endif