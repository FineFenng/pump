//
// Created by fenng on 18-8-2.
//

#ifndef PUMP_NET_IO_WATCHER_H_
#define PUMP_NET_IO_WATCHER_H_

#include <functional>


#include <pump/net/EventLoop.h>
#include <pump/net/Watcher.h>


namespace pump { namespace net
{
class EventLoop;

class Socket;

class TcpConnection;

class IOWatcher : public Watcher
{
public:
	IOWatcher(EventLoop* loop, SOCKET fd);

	void handle_callback(const HandleEvent& handle_event) const override;

	int get_index() const override { return index_; }

	void set_index(int index) const override { index_ = index; }

	unsigned int get_events() const override { return events_; }

	void set_events(unsigned int events) override { events_ = events; }

	SOCKET get_fd() const override { return fd_; }

	bool is_readable() const override { return static_cast<bool>(events_ & EventFlag::kIOReadable); }

	bool is_writable() const override { return static_cast<bool>(events_ & EventFlag::kIOWritable); }

	void enable_readable() override
	{
		events_ |= EventFlag::kIOReadable;
		loop_->update_watcher(*this);
	}

	void enable_writable() override
	{
		events_ |= EventFlag::kIOWritable;
		loop_->update_watcher(*this);
	}

	void enable_readable_and_writable()
	{
		events_ = EventFlag::kIO;
		loop_->update_watcher(*this);
	}

	void disable_readable() override
	{
		events_ &= ~EventFlag::kIOWritable;
		loop_->update_watcher(*this);
	}

	void disable_writable() override
	{
		events_ &= ~EventFlag::kIOWritable;
		loop_->update_watcher(*this);
	}

	void disable_all() override
	{
		events_ = 0;
		loop_->update_watcher(*this);
	}

	void set_readable_callback(const ReadableCallback& cb) override { readable_callback_ = cb; }

	void set_writable_callback(const WritableCallback& cb) override { writable_callback_ = cb; }

	void set_erroneous_callback(const ErroneousCallback& cb) override { erroneous_callback_ = cb; }

	EventLoop* get_event_loop()  override { return loop_; } 

private:
	EventLoop* loop_;
	SOCKET fd_;
	mutable int index_;
	unsigned int events_;

private:
	ReadableCallback readable_callback_;
	WritableCallback writable_callback_;
	ErroneousCallback erroneous_callback_;
};
}}

#endif //QPSTEST_IO_HANLE_H
