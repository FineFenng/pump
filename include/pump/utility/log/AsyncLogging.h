#ifndef PUMP_UTILITY_ASYNCLOGGING_H
#define PUMP_UTILITY_ASYNCLOGGING_H

#include <condition_variable>
#include <string>
#include <thread>
#include <vector>

#include <pump/utility/FixedBuffer.h>
#include <pump/utility/thread/CountDownLatch.h>
#include <pump/utility/thread/ThreadOption.h>

namespace pump
{
namespace utility
{


class AsyncLogging
{
public:
  AsyncLogging(const std::string& base_name, size_t roll_size, int flush_interval)
      : base_name_(base_name),
        roll_size_(roll_size),
        flush_interval_(flush_interval),
        thread_wrapper_(std::bind(&AsyncLogging::thread_function, this), "Async Log Thread"),
        latch_(1),
        is_running_(false) {
    main_buffer_->bzero();
    standby_buffer_->bzero();
    buffer_ptr_list_.reserve(16);
  }

  void append(const BYTE_T* log_line, size_t length);

  void start() {
    is_running_ = true;
    thread_wrapper_.start();
    latch_.wait();
  }

  ~AsyncLogging() {
    if (is_running_) {
      stop();
    }
  }

  void stop();

  void thread_function();



private:
  typedef std::vector<LargeFixedBuffer*> BufferVector;

  std::string base_name_;
  size_t roll_size_;
  int flush_interval_;


  ThreadWrapper thread_wrapper_;
  std::mutex mutex_;
  std::condition_variable cond_;
  CountDownLatch latch_;
  bool is_running_;

  std::vector<LargeFixedBuffer*> buffer_ptr_list_;

  std::unique_ptr<LargeFixedBuffer> main_buffer_;
  std::unique_ptr<LargeFixedBuffer> standby_buffer_;

};


}

}



#endif
