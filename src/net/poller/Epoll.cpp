//
// Created by fenng on 18-8-10.
//


#include <pump/net/WatcherInterface.h>
#include <pump/net/poller/Epoll.h>

namespace pump {
namespace net {

void Epoll::poll(const timeval* tv, TaskList* io_task_list) {
  #define EPOLL_MAX_SIZE 500
  is_quit_ = false;

  while (is_quit_) {
	struct epoll_event event_list[EPOLL_MAX_SIZE];
	int ready_event_count = epoll_wait(backend_fd_, event_list, EPOLL_MAX_SIZE, -1);

	if (ready_event_count > 0) {
	  for (size_t i = 0; i < ready_event_count; ++i) {
		WatcherInterface* handle = static_cast<WatcherInterface*>(event_list[i].data.ptr);
		assert(handle);
		handle->handle_callback(revents);
	  }
	}
  }
  is_quit_ = true;
}

void Epoll::add_interests(const WatcherInterface& handle) {

  if (handle.get_index() < 0) {

	struct epoll_event ev;
	ev.events = handle.get_events();
	ev.data.ptr = const_cast<WatcherAbstract*>(&handle);
	event_list_.push_back(ev);

	epoll_ctl(backend_fd_, EPOLL_CTL_ADD, handle.get_fd(), &ev);
	handle.set_index(static_cast<int>(event_list_.size()) - 1);
	//utility::write_log("[%s][%s][%s:%d]\n", __FILE__, __func__, "add socket :", handle->get_fd());
  }
}

void Epoll::modify_interests(const WatcherAbstract& handle) {
  int index = handle.get_index();
  if (index >= 0) {

	struct epoll_event& ev = event_list_[handle.get_index()];
	ev.events = handle.get_events();

	epoll_ctl(backend_fd_, EPOLL_CTL_MOD, handle.get_fd(), &ev);

  }
}

void Epoll::delete_interests(const WatcherAbstract& handle) {
  int index = handle.get_index();
  if (index >= 0) {

	struct epoll_event& ev = event_list_[index];
	if (index != event_list_.size() - 1) {
	  WatcherAbstract* back_handle = static_cast<WatcherAbstract*>(event_list_.back().data.ptr);
	  back_handle->set_index(index);
	  std::swap(event_list_[index], event_list_.back());
	}

	assert(&handle == event_list_.back().data.ptr);
	epoll_ctl(backend_fd_, EPOLL_CTL_DEL, handle.get_fd(), nullptr);

	event_list_.pop_back();
	//utility::write_log("[%s][%s][%s:%d]\n", __FILE__, __func__, "delete socket :", handle->get_fd());
  }
}

void Epoll::init() {

#ifdef EPOLL_CLOEXEC
  backend_fd_ = epoll_create1(EPOLL_CLOEXEC);
  if (backend_fd_ < 0 && (errno == EINVAL || errno == ENOSYS))
#endif
  {
	backend_fd_ = epoll_create(256);
  }
  assert(backend_fd_ != -1);
  fcntl(backend_fd_, F_SETFD, FD_CLOEXEC);

}

}
}


