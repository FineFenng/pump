#ifndef PUMP_NET_CHANNELCONTEXT_H_
#define PUMP_NET_CHANNELCONTEXT_H_

#include <pump/Common.h>

namespace pump { namespace net {
enum HandlerFlag {
  kNone = 0x00,
  kActivity = 0x01,
  kInput = 0x02,
  kOutput = 0x04,
  kClosed = 0x08,
  kRemoved = 0x0F,
};

class WatcherInterface;
class Handler;
class EventLoop;
class ChannelPipeline;

class ChannelContext {
  friend ChannelPipeline;
 public:
  ChannelContext(WatcherInterface* watcher, Handler* handler);
 private:

  ChannelContext* find_next(HandlerFlag flag) {
	ChannelContext* ctx = this;

	do {
	  ctx = ctx->next_; // TODO ???
	  PUMP_ASSERT(ctx != nullptr);
	} while (!(ctx->flag_ & flag));

	return ctx;
  }

  ChannelContext* find_prev(HandlerFlag flag) {
	ChannelContext* ctx = this;

	do {
	  ctx = ctx->prev_;
	  PUMP_ASSERT(ctx != nullptr);

	} while (!(ctx->flag_ & flag));
	return ctx;
  }

 private:

  WatcherInterface* watcher_;
  Handler* handler_;
  EventLoop* loop_;

  ChannelContext* prev_;
  ChannelContext* next_;
  HandlerFlag flag_;

 private:
};
}}

#endif
