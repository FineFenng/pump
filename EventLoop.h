//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_EVENTLOOP_H
#define QPSTEST_EVENTLOOP_H

#include "Platform.h"
#include <functional>
#include <map>
#include <vector>
#ifdef  Q_OS_MACOS
#include <sys/event.h>
#endif

#include <thread>
#include <boost/noncopyable.hpp>


class Handle;

class EventLoop : boost::noncopyable
{
public:
#ifdef Q_OS_MACOS
    typedef std::function<void(struct kevent*)> HandleCallBackFunction;
#endif

#ifdef Q_OS_LINUX
    typedef std::function<void(struct event*)> HandleCallBackFunction;
#endif


public:
    EventLoop();

    ~EventLoop();

    void run();

    void add_handle(Handle *handle);

    void modify_handle(Handle *handle);

    void delete_handle(Handle *handle);


private:
    bool isInInitThread();


private:
#ifdef Q_OS_MACOS
    typedef std::vector<struct kevent> EventList;
#endif
#ifdef Q_OS_LINUX
    typedef std::vector<struct epoll_event> EventList;
#endif
    //typedef std::map<int, Handle*> HandleMap;

    EventList event_list_;
    EventList already_event_list_;

    //HandleMap handle_map_;

private:
    bool is_looping_;
    bool is_quit_;
private:
    int ep_fd_;
    std::thread::id threadId_;
};


#endif //QPSTEST_EVENTLOOP_H
