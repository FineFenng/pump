#include <pump/net/ChannelContext.h>
#include <pump/net/Watcher.h>

namespace pump {namespace net
{
ChannelContext::ChannelContext(Watcher* watcher, Handler* handler)
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
