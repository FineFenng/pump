#include <pump/net/ChannelContext.h>
#include <pump/net/WatcherInterface.h>

namespace pump {namespace net {
	ChannelContext::ChannelContext(EventLoop* loop, Channel* channel, HandlerInterface* handler)
		: loop_(loop),
		  channel_(channel),
		  handler_(handler),
		  prev_(nullptr),
		  next_(nullptr),
		  flag_(kNone) {


	}
}}
