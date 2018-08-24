#ifndef PUMP_NET_EVENTLOOPTHREADPOOL_H_
#define PUMP_NET_EVENTLOOPTHREADPOOL_H_

#include <algorithm>
#include <pump/net/EventLoopThread.h>


namespace pump {namespace net
{
class EventLoopThreadPool
{
public:
	EventLoopThreadPool(int event_loop_num)
		: event_loop_num_(event_loop_num)
		, loop_index_(0)
	{
		init_pool();
	}

	EventLoop* get_next_event_loop()
	{
		loop_index_ %= event_loop_threads_.size();
		return event_loop_threads_[loop_index_++]->get_event_loop();
	}

private :
	void init_pool()
	{
		for (int i = 0; i < event_loop_num_; ++i) {
			std::unique_ptr<EventLoopThread> event_loop_thread(new EventLoopThread());
			event_loop_threads_.push_back(std::move(event_loop_thread));
		}
	}

private
:
	int event_loop_num_;
	int loop_index_;
	std::vector<std::unique_ptr<EventLoopThread>> event_loop_threads_;
};
}}
#endif
