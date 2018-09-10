#ifndef PUMP_NET_EVENTLOOPTHREADPOOL_H_
#define PUMP_NET_EVENTLOOPTHREADPOOL_H_

#include <algorithm>
#include <pump/net/EventLoopThread.h>
#include <pump/utility/thread/CountDownLatch.h>


namespace pump {namespace net
{
class EventLoopThreadPool
{
public:
	EventLoopThreadPool(int event_loop_num)
		: event_loop_num_(event_loop_num),
		count_down_latch_(event_loop_num),
		loop_index_(0)
	{
		init_pool();
	}

	PUMP_DECLARE_NON_COPYABLE(EventLoopThreadPool)
	PUMP_DECLARE_NON_MOVABLE(EventLoopThreadPool)

	~EventLoopThreadPool()
	{

		
		
	}


	EventLoop* get_next_event_loop()
	{
		loop_index_ %= event_loop_threads_.size();
		return event_loop_threads_[loop_index_++]->get_event_loop();
	}

	void finish_one_thread_init()
	{
		count_down_latch_.count_down();
	}

private :
	enum ThreadStyle
	{
	};
	void init_pool()
	{
		for (int i = 0; i < event_loop_num_; ++i) {
			std::unique_ptr<EventLoopThread> event_loop_thread(new EventLoopThread(this, i + 1));
			event_loop_threads_.push_back(std::move(event_loop_thread));
		}
		count_down_latch_.wait();
	}

private
:
	int event_loop_num_;
	std::vector<std::unique_ptr<EventLoopThread>> event_loop_threads_;
	pump::utility::CountDownLatch count_down_latch_;
	int loop_index_;
};
}}
#endif
