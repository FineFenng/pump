#ifndef PUMP_NET_EVENTLOOPTHREAD_H_
#define PUMP_NET_EVENTLOOPTHREAD_H_

#include <memory>
#include <thread>

#include <pump/net/EventLoop.h>


namespace pump { namespace net
{
class EventLoopThreadPool;

class EventLoopThread
{
public:
	EventLoopThread(EventLoopThreadPool* event_loop_thread_pool, int work_num)
		: loop_(nullptr),
		event_loop_thread_pool_(event_loop_thread_pool),
		is_finish_init_(false),
		work_num_(work_num)

	{
		std::thread th(std::bind(&EventLoopThread::run, this));
		thread_ = std::move(th);
	}

	~EventLoopThread()
	{
		if (thread_.joinable()) {
			thread_.join();
		}
	}

PUMP_DECLARE_NON_COPYABLE(EventLoopThread)
PUMP_DECLARE_NON_MOVABLE(EventLoopThread)

	EventLoop* get_event_loop() const { return loop_; }

	void notify_event_loop_thread_pool() const;

private:
	void run();


private:
	EventLoop* loop_;
	EventLoopThreadPool* event_loop_thread_pool_;
	mutable std::thread thread_;
	bool is_finish_init_;
	int work_num_;
};
}}

#endif
