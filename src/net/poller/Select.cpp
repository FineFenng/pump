//
// Created by fenng on 18-8-13.
//

#include <pump/Common.h>

#include <algorithm>

#include <pump/net/WatcherInterface.h>
#include <pump/net/watcher/IOWatcher.h>
#include <pump/utility/log/Logger.h>

namespace pump {namespace net
{
void Select::poll(struct timeval* tv, TaskList* io_task_list)
{
	ReadableList readable_list_vr;
	WritableList writable_list_vr;

	::memcpy(&readable_list_vr, &readable_list_, sizeof(readable_list_));
	::memcpy(&writable_list_vr, &writable_list_, sizeof(writable_list_));

	int ready_event_count = ::select(max_fd_ + 1, &readable_list_vr, &writable_list_vr, nullptr, nullptr);
	const int saved_errno = errno;
	if (ready_event_count > 0) {
		LOG_TRACE << "Current ready socket fd number: " << ready_event_count;
		for (size_t i = 0; i < watcher_list_.size() && ready_event_count; ++i) {
			unsigned int revents = 0;
			std::reference_wrapper<const watcher> watcher = watcher_list_[i];
			const SOCKET fd = watcher.get().get_fd();
			if (FD_ISSET(fd, &readable_list_vr)) {
				revents |= static_cast<unsigned int>(EventFlag::kIOReadable);
				LOG_TRACE << "Socket fd:" << fd << " has readable event happen.";
			}
			if (FD_ISSET(fd, &writable_list_vr)) {
				revents |= static_cast<unsigned int>(EventFlag::kIOWritable);
				LOG_TRACE << "Socket fd:" << fd << " has writable event happen.";
			}
			if (revents > 0) {
				io_task_list->push_back(std::bind(&watcher::handle_callback, watcher, revents));
				--ready_event_count;
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

void Select::init_backend()
{
	FD_ZERO(&readable_list_);
	FD_ZERO(&writable_list_);
}

void Select::add_interests(const watcher& watcher)
{
	if (watcher.get_index() < 0) {
		const SOCKET fd = watcher.get_fd();
#ifdef PUMP_PLATFORM_WIN
		max_fd_ = max(fd, max_fd_);
#else
	max_fd_ = std::max(fd, max_fd_);
#endif
		if (watcher.get_events() & EventFlag::kIOReadable) {
			FD_SET(fd, &readable_list_);
		}
		if (watcher.get_events() & EventFlag::kIOWritable) {
			FD_SET(fd, &writable_list_);
		}
		watcher_list_.push_back(std::ref(watcher));
		watcher.set_index(static_cast<int>(watcher_list_.size()) - 1);
		LOG_TRACE << "Interest new socket fd: " << fd;
	}
}

void Select::modify_interests(const watcher& watcher)
{
	assert(watcher.get_index() >= 0);
	const SOCKET fd = watcher.get_fd();
	if (watcher.get_events() & EventFlag::kIOReadable) {
		FD_SET(fd, &readable_list_);
	}
	else {
		FD_CLR(fd, &readable_list_);

	}
	if (watcher.get_events() & EventFlag::kIOWritable) {
		FD_SET(fd, &writable_list_);
	}
	else {
		FD_CLR(fd, &writable_list_);
	}
}

void Select::delete_interests(const watcher& watcher)
{
	const int index = watcher.get_index();
	const SOCKET fd = watcher.get_fd();
	assert(index >= 0);
	std::reference_wrapper<const watcher>& handle_ref = watcher_list_[index];
	if (index != watcher_list_.size() - 1) {
		std::reference_wrapper<const watcher>& back_handle_ref = watcher_list_.back();
		back_handle_ref.get().set_index(index);
		std::swap(handle_ref, back_handle_ref);
	}
	FD_CLR(fd, &readable_list_);
	FD_CLR(fd, &writable_list_);
	watcher_list_.pop_back();
}
}}
