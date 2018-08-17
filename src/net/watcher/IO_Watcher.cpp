//
// Created by fenng on 18-8-2.
//


#include <pump/net/WatchAbstract.h>
#include <pump/net/watcher/IO_Watcher.h>
#include <pump/utility/log/Logger.h>


namespace pump { namespace net
{
IO_Watcher::IO_Watcher(class EventLoop* loop, int fd)
	: loop_(loop), fd_(fd), index_(-1), events_(0)

{
}

void IO_Watcher::handle_callback(const HandleEvent& handle_event) const
{
	if (handle_event & IO_Flag::kIOReadable) {
		if (readable_callback_) { readable_callback_(); }
	}

	if (handle_event & IO_Flag::kIOWritable) {
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
