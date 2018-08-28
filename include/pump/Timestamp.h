//
// Created by finefenng on 2018/8/26.
//

#ifndef PUMP_TIMESTAMP_H_
#define PUMP_TIMESTAMP_H_

#include <chrono>
#include <string>
#include <ctime>
#include <iostream>

#include <pump/Common.h>

namespace pump
{
class Timestamp
{
public:
	typedef std::chrono::system_clock Clock;
	typedef std::chrono::microseconds MicrosecondDuration;
	typedef std::chrono::time_point<Clock, MicrosecondDuration> TimePoint;

public:
	Timestamp(const TimePoint& time_point)
		: time_point_(time_point) { }

	const TimePoint& get_time_point() const { return time_point_; }

	static Timestamp now()
	{
		const TimePoint current_point = std::chrono::time_point_cast<MicrosecondDuration>(Clock::now());
		return Timestamp{current_point};
	}

	std::string get_time_point_string() const
	{
		char time_buffer[64] = {0};
		struct tm time;

		const auto seconds = time_point_.time_since_epoch().count() / 1000000;

		const long nano_seconds = time_point_.time_since_epoch().count() % 1000000;

#ifdef PUMP_PLATFORM_WIN
		gmtime_s(&time, &seconds);
#else
		gmtime_r(&seconds, &time);
#endif
		const int count = strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", &time);
		snprintf(time_buffer + count, sizeof(time_buffer) - count, ".%ld", nano_seconds);
		return time_buffer;
	}

private:
	TimePoint time_point_;
};

/*
inline Timestamp operator+(const Timestamp& lhs, double add_second)
{
	const auto tmp_time = static_cast<long>(add_second);
	const auto nano_seconds = static_cast<long long>(add_second * 1e9) + static_cast<long long>((add_second -
		tmp_time) * 1e9);
	const Timestamp::TimePoint::duration nano_duration(nano_seconds);
	return Timestamp{lhs.get_time_point() + nano_duration};
}
*/
}

#endif //PUMP_TIMESTAMP_H_
