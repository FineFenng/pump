#ifndef PUMP_MACROS_H_
#define PUMP_MACROS_H_

//#include <pump/Types.h>



//#define PUMP_CPLUSPLUS __cplusplus

#define WAWO_MALLOC_H_RESERVE (128)

#define PUMP_MIN(A,B)              ((A)<(B)?(A):(B))
#define PUMP_MAX(A,B)              ((A)>(B)?(A):(B))

#define PUMP_MIN2(A,B)             ((A)<(B)?(A):(B))
#define PUMP_MAX2(A,B)             ((A)>(B)?(A):(B))

#define PUMP_MIN3(A,B,C)           PUMP_MIN2(PUMP_MIN2(A,B),(C))
#define PUMP_MAX3(A,B,C)           PUMP_MAX2(PUMP_MAX2(A,B),(C))

#ifdef PUMP_NO_CXX11_DELETED_FUNC
#define PUMP_DECLARE_NONCOPYABLE(__classname__) \
	private: \
	__classname__(const __classname__&); \
	__classname__& operator=(const __classname__&);
#endif

#define PUMP_DECLARE_NONCOPYABLE(__classname__) \
	public: \
	__classname__(const __classname__&) = delete; \
	__classname__& operator=(const __classname__&) = delete;

#define PUMP_DECLAR_NONMOVABLE(__classname__) \
	public:\
	__classname__(__classname__&&) = delete; \
	__classname__& operator=(__classname__&&) = delete;

#define PUMP_DECLAE_CALLBACK_FUNCTIONR(T,...) typedef std::function<T(__VA_ARGS__)>


#endif