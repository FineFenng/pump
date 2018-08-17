//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_EVENTLOOP_H
#define QPSTEST_EVENTLOOP_H

#include <functional>
#include <map>
#include <vector>
#include <thread>


namespace pump
{

namespace net
{

class BackendAbstract;
class WatchAbstract;

class EventLoop 
{
 public:
  EventLoop();

  ~EventLoop();

  EventLoop(const EventLoop&) = delete;
  EventLoop& operator=(const EventLoop&) = delete;

  EventLoop(EventLoop&&) = delete;
  EventLoop& operator=(EventLoop&&) = delete;

  void run();

  void update_handle(const WatchAbstract& handle) const;

  void remove_handle(const WatchAbstract& handle) const;

 private:
  void add_handle(const WatchAbstract& handle) const;

  void modify_handle(const WatchAbstract& handle) const;

  void delete_handle(const WatchAbstract& handle) const;

 private:
  bool isInInitThread();

 private:
  bool is_looping_;
  std::unique_ptr<BackendAbstract> backend_;
 private:
  std::thread::id threadId_;
};

}
}

#endif //QPSTEST_EVENTLOOP_H
