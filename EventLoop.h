//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_EVENTLOOP_H
#define QPSTEST_EVENTLOOP_H

#include <functional>
#include <map>
#include <vector>
#include <sys/event.h>
#include <thread>


class Handle;

class EventLoop
{
public:
    typedef std::function<void(struct kevent*)> HandleCallBackFunction;
public:
    EventLoop();

    ~EventLoop();

    void run();


    void registerHandle(Handle* handle);

    void updateHandle(Handle* handle);

    void disableHandle(Handle* handle);

    void deleteHandle(Handle* handle);

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
