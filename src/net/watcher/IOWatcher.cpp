//
// Created by fenng on 18-8-2.
//


#include <pump/net/watcher/IOWatcher.h>
#include <pump/utility/log/LogLine.h>


namespace pump { namespace net
{
IOWatcher::IOWatcher(class EventLoop* loop, SOCKET fd)
	: loop_(loop), fd_(fd), index_(-1), events_(0)

{ }

void IOWatcher::handle_callback(const HandleEvent& handle_event) const
{
	if (handle_event & EventFlag::kIOReadable) {
		if (readable_callback_) { readable_callback_(); }
	}

	if (handle_event & EventFlag::kIOWritable) {
		if (writable_callback_) { writable_callback_(); }
	}

	/*
	if (events & EPOLLERR) {
		utility::write_log("%s\n", "EPOLLERR");
		if (erroneous_callback_) { erroneous_callback_(); }
	}
	*/
}
}}
