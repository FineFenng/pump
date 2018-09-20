#ifndef PUMP_NET_CHANNELPIPELINE_H
#define PUMP_NET_CHANNELPIPELINE_H

#include <pump/net/WatcherInterface.h>
#include <pump/net/ChannelContext.h>

namespace pump { namespace net
{

class EventLoop;
class Handler;

class ChannelPipeline
{
public:

	explicit ChannelPipeline(EventLoop* loop)
		: loop_(loop),
		head_(nullptr),
		tail_(nullptr)
	{}

	void init();
	void deinit();

	void push_back_handler(WatcherInterface* watcher, Handler* handler)
	{
		ChannelContext* ctx = new ChannelContext(watcher, handler);



	}


private:


	EventLoop* loop_;
	ChannelContext* head_;
	ChannelContext* tail_;
};
}}
#endif
