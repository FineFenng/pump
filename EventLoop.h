//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_EVENTLOOP_H
#define QPSTEST_EVENTLOOP_H

#include <functional>
#include <map>
#include <vector>
#include <sys/event.h>

class Handle;
class EventLoop
{
public:
    typedef std::function<void(int fd, int revents)> HandleCallBackFunction;
public:
    EventLoop();
    ~EventLoop();
    void run();


    void registerHandleCallBackFunction(Handle* handle);
    void updateHandleCallBackFunction(Handle* handle);
    void cancelHandleCallBackFunction(Handle* handle);


private:
    typedef std::vector<struct kevent> HandleList;
    HandleList handList_;
    HandleList alreadyHandList_;


};


#endif //QPSTEST_EVENTLOOP_H
