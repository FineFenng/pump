//
// Created by fenng on 18-8-10.
//

#ifndef PUMP_EPOLL_H
#define PUMP_EPOLL_H

#include <pump/Common.h>
#include <cassert>
#include <cerrno>
#include <pump/net/BackendAbstract.h>

namespace pump
{

namespace net
{

class EventLoop;

class Epoll : public BackendAbstract
{
 public:
  explicit Epoll(EventLoop *loop)
	  : loop_(loop), backend_fd_(-1), is_quit_(true)
  {}

  void poll() override;

  void init_backend() override;

  void add_interests(const WatchAbstract &handle) override;
  void modify_interests(const WatchAbstract &handle) override;
  void delete_interests(const WatchAbstract &handle) override;

 private:

  EventLoop *loop_;

  int backend_fd_;
  bool is_quit_;
 private:
  typedef std::vector<struct epoll_event> EventList;
  EventList event_list_;

};
}
}

#endif //PUMP_EPOLL_H
