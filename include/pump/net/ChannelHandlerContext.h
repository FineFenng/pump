#ifndef PUMP_NET_CHANNELCONTEXT_H_
#define PUMP_NET_CHANNELCONTEXT_H_

#include <pump/Common.h>
#include <pump/net/EventLoop.h>
#include <pump/net/InvokerInterface.h>
#include <pump/net/ChannelHandlerInterface.h>
#include <memory>

namespace pump { namespace net {
enum class HandlerFlag {
  kNone = 0x00,
  kActivity = 0x01,
  kInput = 0x02,
  kOutput = 0x04,
  kClosed = 0x08,
  kRemoved = 0x10,
};

class Channel;
class ChannelHandlerInterface;
class EventLoop;
class ChannelPipeline;

class ChannelHandlerContext : public ActivityInvokerInterface,
                              public InboundInvokerInterface,
                              public OutboundInvokerInterface {
  friend ChannelPipeline;
 public:
  ChannelHandlerContext(EventLoop* loop, Channel* channel, ChannelHandlerInterface* handler);

  void fire_connected() override {
    ChannelHandlerContext* _context = ChannelHandlerContext::find_next(HandlerFlag::kActivity);
    PUMP_ASSERT(_context != nullptr);
    _context->invoke_connected();
  }

  void invoke_connected() const {
    auto* _active_handler = dynamic_cast<ActivityHandlerInterface*>(handler_);
    PUMP_ASSERT(_active_handler);
    _active_handler->connected(*this);
  }

  void fire_closed() override {
    ChannelHandlerContext* _context = ChannelHandlerContext::find_next(HandlerFlag::kActivity);
    PUMP_ASSERT(_context != nullptr);
    _context->invoke_closed();
  }

  void invoke_closed() const {
    auto* _active_handler = dynamic_cast<ActivityHandlerInterface*>(handler_);
    PUMP_ASSERT(_active_handler);
    _active_handler->closed(*this);
  }

  void fire_readable_shutdown() override {
    ChannelHandlerContext* _context = ChannelHandlerContext::find_next(HandlerFlag::kActivity);
    PUMP_ASSERT(_context != nullptr);
    _context->invoke_readable_shutdown();
  }

  void invoke_readable_shutdown() const {
    auto* _active_handler = dynamic_cast<ActivityHandlerInterface*>(handler_);
    PUMP_ASSERT(_active_handler);
    _active_handler->readable_shutdown(*this);
  }

  void fire_writable_shutdown() override {
    ChannelHandlerContext* _context = ChannelHandlerContext::find_next(HandlerFlag::kActivity);
    PUMP_ASSERT(_context != nullptr);
    _context->invoke_writable_shutdown();
  }

  void invoke_writable_shutdown() const {
    auto* _active_handler = dynamic_cast<ActivityHandlerInterface*>(handler_);
    PUMP_ASSERT(_active_handler);
    _active_handler->writable_shutdown(*this);
  }

  void fire_write_blocked() override {
    ChannelHandlerContext* _context = ChannelHandlerContext::find_next(HandlerFlag::kActivity);
    PUMP_ASSERT(_context != nullptr);
    _context->invoke_write_blocked();
  }

  void invoke_write_blocked() const {
    auto* _active_handler = dynamic_cast<ActivityHandlerInterface*>(handler_);
    PUMP_ASSERT(_active_handler);
    _active_handler->writable_shutdown(*this);
  }

  void fire_write_unblocked() override {
    ChannelHandlerContext* _context = ChannelHandlerContext::find_next(HandlerFlag::kActivity);
    PUMP_ASSERT(_context != nullptr);
    _context->invoke_write_unblocked();
  }

  void invoke_write_unblocked() const {
    auto* _active_handler = dynamic_cast<ActivityHandlerInterface*>(handler_);
    PUMP_ASSERT(_active_handler);
    _active_handler->write_unblocked(*this);
  }

  void fire_read(const Packet& income) override {
    ChannelHandlerContext* _context = ChannelHandlerContext::find_next(HandlerFlag::kInput);
    PUMP_ASSERT(_context != nullptr);
    _context->invoke_read(income);
  }

  void invoke_read(const Packet& income) const {
    auto* _inbound_handler = dynamic_cast<InboundHandlerInterface*>(handler_);
    PUMP_ASSERT(_inbound_handler);
    _inbound_handler->read(*this, income);
  }

 public:
  void write(const Packet& income) override {
    auto* _outbound_handler = dynamic_cast<OutboundInvokerInterface*>(handler_);
    loop_->push_back_task(std::bind(&OutboundInvokerInterface::write, _outbound_handler, income));
  }

  void shutdown_read() override {

  }

  void shutdown_write() override {

  }

  void flush() override {
    loop_->push_back_task(std::bind(&ChannelHandlerContext::flush, this));
  }

  void invoke_flush() const {
    auto* outbound_handler = dynamic_cast<OutboundHandlerInterface*>(handler_);
    outbound_handler->flush(*this);
  }

 private:
  void _flush() {
    ChannelHandlerContext* _outbound_context = ChannelHandlerContext::find_prev(HandlerFlag::kOutput);
    _outbound_context->invoke_flush();
  }

 private:

  ChannelHandlerContext* find_next(HandlerFlag flag) {
    ChannelHandlerContext* ctx = this;

    do {
      ctx = ctx->next_; // TODO ???
      PUMP_ASSERT(ctx != nullptr);
    } while (!(ctx->flag_ & flag));

    return ctx;
  }

  ChannelHandlerContext* find_prev(HandlerFlag flag) {
    ChannelHandlerContext* ctx = this;

    do {
      ctx = ctx->prev_;
      PUMP_ASSERT(ctx != nullptr);

    } while (!(ctx->flag_ & flag));
    return ctx;
  }

 private:

  EventLoop* loop_;
  Channel* channel_;
  ChannelHandlerInterface* handler_;
  ChannelHandlerContext* prev_;
  ChannelHandlerContext* next_;
  HandlerFlag flag_;

 private:
};
}}

#endif
