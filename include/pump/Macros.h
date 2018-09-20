#ifndef PUMP_MACROS_H_
#define PUMP_MACROS_H_

#include <cassert>

#include <pump/config/Platform.h>


namespace pump
{

void assert_failed(const char* error, const char* file, int line, const char* function, ...);
//#define PUMP_CPLUSPLUS __cplusplus

#define PUMP_MALLOC_H_RESERVE (128)

#define PUMP_MIN(A,B)              ((A)<(B)?(A):(B))
#define PUMP_MAX(A,B)              ((A)>(B)?(A):(B))

#define PUMP_MIN2(A,B)             ((A)<(B)?(A):(B))
#define PUMP_MAX2(A,B)             ((A)>(B)?(A):(B))

#define PUMP_MIN3(A,B,C)           PUMP_MIN2(PUMP_MIN2(A,B),(C))
#define PUMP_MAX3(A,B,C)           PUMP_MAX2(PUMP_MAX2(A,B),(C))

#define PUMP_NEGATIVE(A)           ((A<0)?(A):(-(A)))

#define PUMP_TV_SET(tv,t) \
	do { tv.tv_sec = static<long>(t); tv.tv_usec = static_cast<long>((t - tv.tv_sec) * 1e6); } while (0)
#define PUMP_TS_SET(tv,t) \
	do { tv.tv_sec = static<long>(t); tv.tv_usec = static_cast<long>((t - tv.tv_sec) * 1e9); } while (0)

#ifdef PUMP_NO_CXX11_DELETED_FUNC
#define PUMP_DECLARE_DELETE_COPYABLE(__classname__) \
	private: \
	__classname__(const __classname__&); \
	__classname__& operator=(const __classname__&);
#endif

#define PUMP_DECLARE_DELETE_COPYABLE(__classname__) \
	public: \
	__classname__(const __classname__&) = delete; \
	__classname__& operator=(const __classname__&) = delete;

#define PUMP_DECLARE_DEFAULT_COPYABLE(__classname__) \
	public: \
	__classname__(const __classname__&) = default; \
	__classname__& operator=(const __classname__&) = default;

#define PUMP_DECLARE_DELETE_MOVABLE(__classname__) \
	public:\
	__classname__(__classname__&&) = delete; \
	__classname__&& operator=(__classname__&&) = delete;

#define PUMP_DECLARE_DEFAULT_MOVABLE(__classname__) \
	public:\
	__classname__(__classname__&&) = default; \
	__classname__& operator=(__classname__&&) = default;


#define PUMP_DECLARE_CALLBACK_FUNCTION(T,...)  typedef std::function<T(__VA_ARGS__)>

#ifdef _DEBUG
	#ifdef PUMP_PLATFORM_WIN
		#define PUMP_ASSERT(x, ...) ((void) ((x)? (void)0: pump::assert_failed(#x, __FILE__, __LINE__, __FUNCTION__, "" __VA_ARGS__)))
	#endif
#else
	#define PUMP_ASSERT(x, ...) assert(x)
#endif

}

#endif
