//
// Created by fenng on 18-8-10.
//

#ifndef PUMP_EPOLL_H
#define PUMP_EPOLL_H

#include <cassert>
#include <cerrno>

#include <vector>

#include <pump/net/Poller.h>

namespace pump
{

namespace net
{

class EventLoop;

class Epoll : public Poller
{
 public:
  explicit Epoll(EventLoop* loop)
	  : loop_(loop), backend_fd_(-1), is_quit_(true)
  {}

  void poll(const timeval* tv, TaskList* io_task_lit) override;

  void init_backend() override;

  void add_interests(const Watcher& handle) override;
  void modify_interests(const Watcher& handle) override;
  void delete_interests(const Watcher& handle) override;

 private:
  EventLoop* loop_;
  int backend_fd_;
  bool is_quit_;

 private:
  typedef std::vector<struct epoll_event> EventList;
  EventList event_list_;

};
}
}

#endif //PUMP_EPOLL_H
