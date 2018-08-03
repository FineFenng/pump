//
// Created by fenng on 18-8-2.
//

#include "IO_Handle.h"

IO_Handle::IO_Handle(EventLoop *loop, int fd)
: loop_(loop), fd_(fd), index_(-1), events_(0),
  readable_callback_(),writable_callback_()
{ }

void IO_Handle::handle_callback(struct epoll_event *ev)
{
    uint32_t events = ev->events;

    if (events & EPOLLIN)
    {
        if (readable_callback_) { readable_callback_(); }
    }

    if (events & EPOLLOUT)
    {
        if (writable_callback_) { writable_callback_(); }
    }

}
