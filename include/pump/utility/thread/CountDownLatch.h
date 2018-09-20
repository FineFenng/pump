#ifndef  PUMP_UTILITY_COUNTDOWNLATCH_H_
#define  PUMP_UTILITY_COUNTDOWNLATCH_H_

#include <mutex>
#include <condition_variable>

#include <pump/Common.h>

namespace pump { namespace utility
{
class CountDownLatch
{
public:
	explicit CountDownLatch(int count)
		: count_(count)
	{}

	~CountDownLatch() = default;

PUMP_DELETE_COPYABLE_AND_ASSIGN(CountDownLatch)
PUMP_DECLARE_DELETE_MOVABLE(CountDownLatch)

	void wait()
	{
		std::unique_lock<std::mutex> lk(mutex_);
		while (count_ > 0) {
			condition_variable_.wait(lk);
		}
		lk.unlock();
	}

	void count_down()
	{
		std::lock_guard<std::mutex> lk(mutex_);
		--count_;

		if (count_ == 0) {
			condition_variable_.notify_all();
		}
	}

private:

	std::mutex mutex_;
	std::condition_variable condition_variable_;
	int count_;
};
}}

#endif
