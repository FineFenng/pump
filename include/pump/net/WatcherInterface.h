//
// Created by finefenng on 2018/7/5.
//

#ifndef PUMP_NET__WATCHER_H_
#define PUMP_NET__WATCHER_H-

#include <cstdlib>
#include <functional>

#include <pump/Common.h>


typedef unsigned int HandleEvent;

namespace pump { namespace net
{
class EventLoop;

class WatcherInterface
{
public:

	PUMP_DECLARE_CALLBACK_FUNCTION(void) ReadableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void) WritableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void) ErroneousCallback;

public:

	virtual ~WatcherInterface()  = default;

	virtual void handle_callback(const HandleEvent& handle_event) const = 0;

	virtual int get_index() const = 0;

	virtual void set_index(int index) const = 0;

	virtual SOCKET get_fd() const = 0;

	virtual unsigned int get_events() const = 0;

	virtual void set_events(unsigned int) = 0;

	virtual bool is_readable() const = 0;

	virtual void enable_readable() = 0;

	virtual void disable_readable() = 0;

	virtual bool is_writable() const = 0;

	virtual void enable_writable() = 0;

	virtual void disable_writable() = 0;

	virtual void disable_all() = 0;

	virtual void set_readable_callback(const ReadableCallback& cb) = 0;

	virtual void set_writable_callback(const WritableCallback& cb) = 0;

	virtual void set_erroneous_callback(const ErroneousCallback& cb) = 0;

	virtual EventLoop* get_event_loop() = 0;
};
}}

#endif //QPSTEST_HANDLE_H
