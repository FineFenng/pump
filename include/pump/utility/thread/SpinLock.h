#ifndef PUMP_UTILITY_SPINLOCK_H_
#define PUMP_UTILITY_SPINLOCK_H_

#include <atomic>
#include <thread>

#include <pump/Common.h>

namespace pump { namespace utility {
class SpinLock {
 PUMP_DECLARE_DELETE_COPYABLE_AND_MOVABLE(SpinLock)
 public:

  SpinLock() = default;
  ~SpinLock() = default;

  void lock() {
	while (flag_.test_and_set(std::memory_order_acquire)) {
	  std::this_thread::yield();
	}
  }

  void unlock() {
	flag_.clear(std::memory_order_acquire);
  }

 private:
  std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};
}}
#endif
