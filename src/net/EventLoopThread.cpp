#include <pump/net/EventLoopThread.h>
#include <pump/net/EventLoopThreadPool.h>

namespace pump { namespace net
{
void EventLoopThread::notify_event_loop_thread_pool() const
{
	event_loop_thread_pool_->finish_one_thread_init();
}

void EventLoopThread::run()
{
	EventLoop vice_loop;
	loop_ = &vice_loop;
	is_finish_init_ = true;
	vice_loop.push_back_task(std::bind(&EventLoopThread::notify_event_loop_thread_pool, this));
	vice_loop.run();
}
}}
