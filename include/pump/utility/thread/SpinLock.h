#ifndef PUMP_UTILITY_SPINLOCK_H_
#define PUMP_UTILITY_SPINLOCK_H_

#include <atomic>
#include <thread>

#include <pump/Common.h>

namespace pump { namespace utility
{
class SpinLock
{
public:

	SpinLock() = default;
	~SpinLock() = default;

PUMP_DELETE_COPYABLE_AND_ASSIGN(SpinLock)
PUMP_DECLARE_DELETE_MOVABLE(SpinLock)

	void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire)) {
			std::this_thread::yield();
		}
	}

	void unlock()
	{
		flag.clear(std::memory_order_acquire);
	}

private:
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
};
}}
#endif
