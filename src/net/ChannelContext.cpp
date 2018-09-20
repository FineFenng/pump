#include <pump/net/ChannelContext.h>
#include <pump/net/WatcherInterface.h>

namespace pump {namespace net
{
ChannelContext::ChannelContext(WatcherInterface* watcher, Handler* handler)
	: watcher_(watcher),
	handler_(handler),
	loop_(watcher_->get_event_loop()),
	prev_(nullptr),
	next_(nullptr),
	flag_(HandlerFlag::kNone)
{
//TODO ??? handler ״̬
	
}
}}
