#ifndef PUMP_TYPES_H_
#define PUMP_TYPES_H_
#include <cstdint>
#include <pump/config/Platform.h>

namespace pump
{
    typedef uint8_t BYTE_T;
	
#ifdef PUMP_PLATFORM_WIN
    typedef unsigned __int64 SOCKET;
#else
    typedef int SOCKET;
#endif
}

#endif