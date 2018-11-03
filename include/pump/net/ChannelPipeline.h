#ifndef PUMP_NET_CHANNELPIPELINE_H
#define PUMP_NET_CHANNELPIPELINE_H

#include <pump/net/WatcherInterface.h>
#include <pump/net/ChannelHandlerContext.h>

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
		ChannelHandlerContext* ctx = new ChannelHandlerContext(watcher, handler);



	}


private:


	EventLoop* loop_;
	ChannelHandlerContext* head_;
	ChannelHandlerContext* tail_;
};
}}
#endif
