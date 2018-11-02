#ifndef PUMP_TYPES_H_
#define PUMP_TYPES_H_
#include <cstdint>
#include <pump/config/Platform.h>

namespace pump
{
    typedef unsigned char BYTE_T;
	
#ifdef PUMP_PLATFORM_WIN
	#if PUMP_IS_ADDRESS_MODE_X64
		typedef unsigned long long SOCKET
	#else
		typedef unsigned long SOCKET;
	#endif
#else
    typedef int SOCKET;
#endif
}

#endif