//
// Created by finefenng on 2018/9/24.
//
#include <pump/utility/thread/ThreadOption.h>

namespace pump {
namespace utility {

std::atomic<int> ThreadWrapper::thread_num_(0);

ThreadWrapper::ThreadWrapper(const ThreadWrapper::ThreadFunction& function, const std::string& thread_name)
    : thread_function_(function),
      thread_(),
      thread_name_(thread_name),
      thread_id_(),
      is_start_(false) {
  set_default_thread_name();
}

ThreadWrapper::ThreadWrapper(const std::string& thread_name)
    : thread_function_(),
      thread_(),
      thread_name_(thread_name),
      thread_id_(),
      is_start_(false) {
  set_default_thread_name();
}

ThreadWrapper::~ThreadWrapper() {
  if (is_start_ && !thread_.joinable()) {
    thread_.detach();
  }
}

void ThreadWrapper::set_default_thread_name() {
  const int num = thread_num_.fetch_add(1);
  if (thread_name_.empty()) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Thread#%d", num);
    thread_name_ = buffer;
  }
}

void ThreadWrapper::start() {
  std::thread temp(thread_function_);
  thread_ = std::move(temp);
  thread_id_ = GetThreadId(thread_.get_id());
  is_start_ = true;
}

void ThreadWrapper::join() {
  if (!thread_.joinable()) {
    thread_.join();
  }
}

} // utility
} // pump

