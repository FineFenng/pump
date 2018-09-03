#ifndef PUMP_UTILITY_THREADOPTION_H_
#define PUMP_UTILITY_THREADOPTION_H_

#include <cstdint>
#include <sstream>
#include <thread>

namespace pump {namespace utility
{

inline uint64_t GetCurrentThreadId()
{
	std::ostringstream out_stream;
	out_stream << std::this_thread::get_id();
	const uint64_t current_thread_id = std::strtoll(out_stream.str().c_str(), nullptr, 16);
	return current_thread_id;
}

}}
#endif
