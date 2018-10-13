#include <pump/net/EventLoopThread.h>
#include <pump/net/EventLoopThreadPool.h>
#include <pump/utility/log/LogLine.h>

namespace pump { namespace net {

void EventLoopThread::notify_event_loop_thread_pool() const {
  event_loop_thread_pool_->finish_one_thread_init();
}

void EventLoopThread::run() {
  EventLoop* loop_ = new EventLoop(work_num_);
  is_finish_init_ = true;
  LOG_DEBUG << "Work thread <" << work_num_ << "> has finished init.";
  loop_->push_back_task(std::bind(&EventLoopThread::notify_event_loop_thread_pool, this));
  loop_->run();
}
}}
