#ifndef PUMP_NET_EVENTLOOPTHREAD_H_
#define PUMP_NET_EVENTLOOPTHREAD_H_

#include <memory>
#include <thread>

#include <pump/net/EventLoop.h>

namespace pump { namespace net
{
class EventLoopThread
{
public:
	EventLoopThread()
		: loop_(nullptr)
		, is_finish_init_(false)
		, is_running_(false)
	{
		std::thread th(std::bind(&EventLoopThread::run, this));
		init();
		thread_ = std::move(th);
	}

	~EventLoopThread()
	{
		if (thread_.joinable()) {
			thread_.join();
		}
	}

	EventLoop* get_event_loop() const
	{
		return loop_;
	}

private:

	void init()
	{
		std::unique_lock<std::mutex> lk(mutex_);
		condition_variable_.wait(lk, [&]() { return is_finish_init_; });
		lk.unlock();
	}

	void run()
	{
			EventLoop vice_loop;
			loop_ = &vice_loop;
			is_finish_init_ = true;
			condition_variable_.notify_one();
			vice_loop.run();
	}

private:
	EventLoop* loop_;
	mutable std::thread thread_;
	std::mutex mutex_;
	std::condition_variable condition_variable_;
	bool is_finish_init_;
	bool is_running_;
};
}}

#endif
