#ifndef PUMP_TIMER_H
#define PUMP_TIMER_H

#include <chrono>
#include <functional>

#include <pump/Common.h>

namespace pump {

	typedef std::chrono::steady_clock::duration TimerDuration;
	typedef std::chrono::steady_clock::time_point TimerPoint;
	typedef std::chrono::steady_clock TimerClock;

	class Timer : public pump::RefBase {
	public:
		typedef std::function<void(const PMRP<Timer>&)> TimerFunction;
	public:
		template<typename Duration, typename TimerFunction, typename ...Args>
		Timer(const Duration& delay, TimerFunction&& function, Args&&... args) 
			: timer_function_(std::forward<TimerFunction>(std::bind(std::forward<TimerFunction>(function), std::forward<>())))
		{
			
		}

	private:
		TimerFunction timer_function_;
		TimerDuration timer_delay_;
		TimerPoint timer_expire_;


	};

}
#endif
