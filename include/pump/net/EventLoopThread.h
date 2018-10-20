#ifndef PUMP_NET_EVENTLOOPTHREAD_H_
#define PUMP_NET_EVENTLOOPTHREAD_H_

#include <memory>
#include <thread>

#include <pump/Common.h>
#include <pump/net/EventLoop.h>
#include <pump/utility/thread/ThreadOption.h>

namespace pump { namespace net {
class EventLoopThreadPool;

class EventLoopThread {
 public:
  EventLoopThread(EventLoopThreadPool* event_loop_thread_pool, int work_num)
      : loop_(nullptr),
        event_loop_thread_pool_(event_loop_thread_pool),
        thread_wrapper_(std::bind(&EventLoopThread::run, this)),
        is_finish_init_(false),
        work_num_(work_num) {
  }

  ~EventLoopThread() {
    if (!loop_) {
      delete loop_;
    }
  }

 PUMP_DECLARE_DELETE_COPYABLE(EventLoopThread)
 PUMP_DECLARE_DELETE_MOVABLE(EventLoopThread)

  void start() {
    thread_wrapper_.start();
  }

  EventLoop* get_event_loop() const { return loop_; }

  void notify_event_loop_thread_pool() const;

 private:
  void run();

 private:
  EventLoop* loop_;
  EventLoopThreadPool* event_loop_thread_pool_;
  pump::utility::ThreadWrapper thread_wrapper_;
  bool is_finish_init_;
  int work_num_;
};
}}

#endif
