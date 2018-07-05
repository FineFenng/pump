//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_EVENTLOOP_H
#define QPSTEST_EVENTLOOP_H

#include <functional>
#include <map>
#include <vector>

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

private:
    typedef std::vector<int> HandleList;
    typedef std::function<void()> CallBackFunction;
    typedef std::map<int, CallBackFunction> EventList;
    HandleList handList_;
    EventList eventList_;


};


#endif //QPSTEST_EVENTLOOP_H
