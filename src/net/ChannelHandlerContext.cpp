#include <pump/net/ChannelHandlerContext.h>
#include <pump/net/WatcherInterface.h>

namespace pump {namespace net {
	ChannelHandlerContext::ChannelHandlerContext(EventLoop* loop, Channel* channel, ChannelHandlerInterface* handler)
		: loop_(loop),
		  channel_(channel),
		  handler_(handler),
		  prev_(nullptr),
		  next_(nullptr),
		  flag_(kNone) {


	}
}}
