//
// Created by fenng on 18-8-13.
//

#ifndef PUMP_POLL_H
#define PUMP_POLL_H

#include <pump/net/BackendAbstract.h>

namespace pump
{

namespace net
{

class EventLoop;

class Epoll : public BackendAbstract
{
 public:
  Epoll(EventLoop *loop)
	  : loop_(loop), backend_fd_(-1), is_quit_(true)
  {}

  void poll(timeval tv) override;

  void init_backend() override;

  void add_interests(const WatcherAbstract &handle) override;
  void modify_interests(const WatcherAbstract &handle) override;
  void delete_interests(const WatcherAbstract &handle) override;

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

#endif //PUMP_POLL_H
