#ifndef PUMP_NET_CHANNELPIPELINE_H_
#define PUMP_NET_CHANNELPIPELINE_H_

#include <pump/net/Watcher.h>
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

	void push_back_handler(Watcher* wathcer, Handler* handler)
	{
		ChannelContext* ctx = new ChannelContext(wathcer, handler);




	}


private:


	EventLoop* loop_;
	ChannelContext* head_;
	ChannelContext* tail_;
};
}}
#endif
