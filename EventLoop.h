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

    void registerHandle(Handle* handle);

    void updateHandle(Handle* handle);

    /*
    void disableHandle(Handle* handle);
     */

    void deleteHandle(Handle* handle);
    
    void removeHandle(Handle* handle);

private:
    bool isInInitThread();


private:
    typedef std::vector<struct kevent> HandleList;
    HandleList handleList_;
    HandleList alreadyHandList_;
private:
    bool isLooping_;
    bool isQuit_;
private:
    std::thread::id threadId_;
};


#endif //QPSTEST_EVENTLOOP_H
