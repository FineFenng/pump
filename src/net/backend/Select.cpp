//
// Created by fenng on 18-8-13.
//

#include <pump/Common.h>

#include <algorithm>
#include <utility>

#include <pump/net/WatchAbstract.h>
#include <pump/net/watcher/IO_Watcher.h>
#include <pump/net/backend/Select.h>
#include <pump/utility/log/Logger.h>

namespace pump {namespace net
{
void Select::poll()
{
	while (is_quit_) {
		for (size_t i = 0; i < event_list_.size(); ++i) {
			const SOCKET fd = event_list_[i].get().get_fd();
			const unsigned int events = event_list_[i].get().get_events();
			if (events & static_cast<unsigned int>(IO_Flag::kIOReadable)) {
				FD_SET(fd, &readable_list_);
			}
			if (events & static_cast<unsigned int>(IO_Flag::kIOWritable)) {
				FD_SET(fd, &writable_list_);
			}
		}
		int ready_event_count = select(max_fd_ + 1,
										&readable_list_, &writable_list_, nullptr, nullptr);
		const int saved_errno = errno;
		if (ready_event_count > 0) {
			for (size_t i = 0; i < event_list_.size() && ready_event_count--; ++i) {
				unsigned int revents = 0;
				const int fd = event_list_[i].get().get_fd();
				if (FD_ISSET(fd, &readable_list_)) {
					revents |= static_cast<unsigned int>(IO_Flag::kIOReadable);
				}
				if (FD_ISSET(fd, &writable_list_)) {
					revents |= static_cast<unsigned int>(IO_Flag::kIOWritable);
				}
				if (revents > 0) {
					event_list_[i].get().handle_callback(revents);
				}
			}
		}
		else if (ready_event_count == 0) {
			LOG_DEBUG << "Select call timeout";
		}
		else {
			if (saved_errno == EINTR) {
				LOG_WARN << "Select has been interrupt by SIGNAL INTERRUPT";
			}
		}

	}
}

void Select::init_backend()
{
	FD_ZERO(&readable_list_);
	FD_ZERO(&writable_list_);
}


void Select::add_interests(const WatchAbstract& handle)
{
	if (handle.get_index() < 0) {
		const SOCKET fd = handle.get_fd();
#ifdef PUMP_PLATFORM_WIN
		max_fd_ = max(fd, max_fd_);
#else
	max_fd_ = std::max(fd, max_fd_);
#endif
		if (handle.get_events() & static_cast<unsigned int>(IO_Flag::kIOReadable)) {
			FD_SET(fd, &readable_list_);
		}
		if (handle.get_events() & static_cast<unsigned int>(IO_Flag::kIOWritable)) {
			FD_SET(fd, &writable_list_);
		}
		event_list_.push_back(std::ref(handle));
	}
}

void Select::modify_interests(const WatchAbstract& handle)
{
	const int index = handle.get_index();
	if (index >= 0) {
		const SOCKET fd = handle.get_fd();
		if (handle.get_events() & static_cast<unsigned int>(IO_Flag::kIOReadable)) {
			FD_CLR(fd, &readable_list_);
		}
		if (handle.get_events() & static_cast<unsigned int>(IO_Flag::kIOWritable)) {
			FD_CLR(fd, &writable_list_);
		}
	}
}

void Select::delete_interests(const WatchAbstract& handle)
{
	const int index = handle.get_index();
	if (index >= 0) {
		std::reference_wrapper<const WatchAbstract>& handle_ref = event_list_[index];
		if (index != event_list_.size() - 1) {
			std::reference_wrapper<const WatchAbstract>& back_handle_ref = event_list_.back();
			back_handle_ref.get().set_index(index);
			std::swap(handle_ref, back_handle_ref);
		}
		event_list_.pop_back();
	}
}
}}
