//
// Created by fenng on 18-8-2.
//

#ifndef QPSTEST_IO_HANLE_H
#define QPSTEST_IO_HANLE_H

#include <memory>
#include <functional>
#include <boost/noncopyable.hpp>
#include <sys/epoll.h>

#include "Handle.h"

class EventLoop;
class Socket;
class TcpConnection;

class IO_Handle : public Handle, private boost::noncopyable
{
public:
    IO_Handle(EventLoop* loop, int fd );

    void handle_callback(struct epoll_event* ev) override;

    int get_index() override { return index_; }

    void set_index(int index) override { index_ = index; }

    unsigned int get_events() override { return events_; }

    void set_events(unsigned int events) override { events_ = events; }

    int get_fd() override { return fd_; }

    bool is_readable() override { return static_cast<bool>(events_ & EPOLLIN); }

    void enable_readable() override { events_ |= EPOLLIN; }

    void disable_readable() override  { events_ &= ~EPOLLIN; }

    bool is_writable() override { return static_cast<bool>(events_ & EPOLLOUT); }

    void enable_writable() override { events_ |= EPOLLOUT; }

    void disable_writable() override { events_ &= ~EPOLLOUT; }

    void disable_all() override { events_ = 0; }

    void set_readable_callback(const ReadableCallback& cb) override { readable_callback_ = cb; }

    void set_writable_callback(const WritableCallback& cb) override { writable_callback_ = cb; }



private:
    EventLoop* loop_;
    int fd_;
    int index_;
    unsigned int events_;

private:
    ReadableCallback readable_callback_;
    WritableCallback writable_callback_;

};


#endif //QPSTEST_IO_HANLE_H
