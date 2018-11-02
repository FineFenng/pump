#ifndef PUMP_UTILITY_THREADOPTION_H_
#define PUMP_UTILITY_THREADOPTION_H_

#include <cstdint>

#include <atomic>
#include <functional>
#include <sstream>
#include <string>
#include <thread>

#include <pump/Common.h>

namespace pump { namespace utility {

inline uint64_t GetThreadId(const std::thread::id thread_id) {
  std::ostringstream out_stream;
  out_stream << thread_id;
  const uint64_t current_thread_id = static_cast<uint64_t>(std::strtoll(out_stream.str().c_str(), nullptr, 16));
  return current_thread_id;
}

inline uint64_t GetCurrentThreadId() {
  return GetThreadId(std::this_thread::get_id());
}

class ThreadWrapper {
 PUMP_DECLARE_DELETE_COPYABLE_AND_MOVABLE(ThreadWrapper)
 public:
  typedef std::function<void()> ThreadFunction;
  explicit ThreadWrapper(const ThreadFunction& function, const std::string& thread_name = std::string());
  explicit ThreadWrapper(const std::string& thread_name = std::string());
  ~ThreadWrapper();

  void set_thread_function(const ThreadFunction& thread_function) {
    thread_function_ = thread_function;
  }

  void start();

  void join();

  bool is_started() const {
    return is_start_;
  }

  uint64_t get_thread_id() const {
    return thread_id_;
  }

  const std::string& get_thread_name() const {
    return thread_name_;
  }

 private:
  void set_default_thread_name();

 private:
  ThreadFunction thread_function_;
  std::thread thread_;
  std::string thread_name_;
  uint64_t thread_id_;
  bool is_start_;
  static std::atomic<int> thread_num_;
};


}}
#endif
