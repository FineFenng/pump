//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_EVENTLOOP_H
#define QPSTEST_EVENTLOOP_H

#include <functional>
#include <map>
#include <vector>

class Handle;
class EventLoop
{
public:
    typedef std::function<void(int fd, int revents)> HandleCallBackFunction;
public:
    EventLoop();
    ~EventLoop();


    void registerHandleCallBackFunction(Handle* handle);
    void updateHandleCallBackFunction(Handle* handle);
    void cancelHandleCallBackFunction(Handle* handle);


private:
    typedef std::vector<int> HandleList;
    typedef std::map<int, HandleCallBackFunction> EventList;
    HandleList handList_;
    EventList eventList_;


};


#endif //QPSTEST_EVENTLOOP_H
