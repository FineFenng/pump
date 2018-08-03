//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_HANDLE_H
#define QPSTEST_HANDLE_H

#include <cstdlib>
#include <functional>


#include "Platform.h"


class Handle
{
public:
    typedef std::function<void()> ReadableCallback;
    typedef std::function<void()> WritableCallback;
#ifdef Q_OS_MACOS
    virtual void handle_callback(struct kevent *event) = 0;
#endif

#ifdef Q_OS_LINUX
    virtual void handle_callback(struct epoll_event* event) = 0;
#endif


    virtual int get_index() = 0;

    virtual void set_index(int index) = 0;

    virtual int get_fd() = 0;

    virtual unsigned int get_events() = 0;

    virtual void set_events(unsigned int events) = 0;

    virtual bool is_readable() = 0;

    virtual void enable_readable()  = 0;

    virtual void disable_readable()  = 0;

    virtual bool is_writable() = 0;

    virtual void enable_writable()  = 0;

    virtual void disable_writable() = 0;

    virtual void disable_all()  = 0;

    virtual void set_readable_callback(const ReadableCallback& cb)  = 0;

    virtual void set_writable_callback(const WritableCallback& cb) = 0;

};


#endif //QPSTEST_HANDLE_H
