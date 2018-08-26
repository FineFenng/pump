//
// Created by finefenng on 2018/8/26.
//

#ifndef PUMP_TIMESTAMP_H_
#define PUMP_TIMESTAMP_H_

#include <chrono>
#include <string>

#include <pump/Common.h>

namespace pump {
class Timestamp {
public:
	typedef std::chrono::steady_clock Clock;
	typedef Clock::time_point         TimePoint;
public:
	Timestamp(const TimePoint& time_point)
		: time_point_(time_point) { }

	const TimePoint& get_time_point() const { return time_point_; }

	static Timestamp now()
	{
		TimePoint now_out = Clock::now();
		return Timestamp(now_out);
	}

	std::string get_time_point_string() const
	{
		char time_buffer[64];
		struct tm time;

		snprintf(time_buffer, sizeof(time_buffer), "" )






	}




private:
	std::chrono::time_point<std::chrono::steady_clock> time_point_;



};

inline Timestamp operator+(const Timestamp& lhs, double add_second)
{
	return lhs.get_time_point() +
		std::chrono::duration<double, std::chrono::seconds>(add_second);
}


}



#endif //PUMP_TIMESTAMP_H_
