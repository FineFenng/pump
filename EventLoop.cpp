//
// Created by finefenng on 2018/7/5.
//

#include "EventLoop.h"


#include <cassert>
#include <boost/noncopyable.hpp>
#include <sys/epoll.h>
#include <vector>

#include "Handle.h"
#include "Acceptor.h"

static const unsigned int MAX_SIZE = 500;

/*
 * struct kevent {
 *     uintptr_t ident;        事件 ID
 *     short     filter;       事件过滤器
 *     u_short   flags;        行为标识
 *     u_int     fflags;       过滤器标识值
 *     intptr_t  data;         过滤器数据
 *     void      *udata;       应用透传数据
 * };
 */

/*
 * struct epoll_event {
 *      uint32_t     events;
 *      epoll_data_t data;
 * }
 *
 * typedef union epoll_data {
 *      void*     ptr;
 *      int        fd;
 *      uint32_t  u32;
 *      uint64_t  u64;
 *  } epoll_data_t;
 *
 */




thread_local EventLoop* t_eventLoop = nullptr;

EventLoop::EventLoop()
        : event_list_(), already_event_list_(),
          is_looping_(false), is_quit_(true), ep_fd_(-1),
          threadId_(std::this_thread::get_id())
{
    if (t_eventLoop == nullptr)
    {
        t_eventLoop = this;
    }

}

void EventLoop::run()
{

#ifdef Q_OS_MACOS
    int ioHandle = kqueue();
    is_looping_ = true;
    isQuit_ = false;

    while (!isQuit_)
    {
        struct kevent* alreadyHandleAddr = nullptr;
        int alreadyHandleNum = kevent(ioHandle, event_list_.data(), static_cast<int>(event_list_.size()),
                                      alreadyHandleAddr, 0, nullptr);
        if (alreadyHandleNum)
        {
            for (int i = 0; i < alreadyHandleNum; ++i)
            {
                if (&alreadyHandleAddr[i] && alreadyHandleAddr[i].udata)
                {
                    Handle* activeHandle = dynamic_cast<Handle*> (alreadyHandleAddr[i].udata);
                    activeHandle->handleCallbackFunction(&alreadyHandleAddr[i]);
                }
            }
        }
    }
#endif

#ifdef Q_OS_LINUX
    ep_fd_ = epoll_create1(EPOLL_CLOEXEC);
    assert(ep_fd_ != -1);
    is_looping_ = true;
    is_quit_ = false;

    while (!is_quit_)
    {
        std::vector<struct epoll_event> event_list(MAX_SIZE);
        int ready_event_count = ::epoll_wait(ep_fd_, event_list.data(), MAX_SIZE, -1);
        if (ready_event_count > 0)
        {
            for(size_t i = 0; i < ready_event_count; ++i)
            {
                Handle* handle = static_cast<Handle*>(event_list[i].data.ptr);
                handle->handle_callback(&event_list[i]);
            }
        }
    }
#endif

    is_looping_ = false;
}

void EventLoop::add_handle(Handle *handle)
{
    if (handle->get_index() < 0)
    {
#ifdef Q_OS_MACOS
        struct kevent event;
        uintptr_t ident = static_cast<uintptr_t>(handle->getFd());
        short filter = handle->getFilter();
        EV_SET(&event, ident, filter, EV_ENABLE, 0, 0, static_cast<void*>(handle));
        event_list_.push_back(event);
        handle->setIndex(static_cast<int>(event_list_.size()));
#endif
#ifdef Q_OS_LINUX
        struct epoll_event ev;
        ev.events = handle->get_events();
        ev.data.ptr = handle;

        epoll_ctl(ep_fd_,EPOLL_CTL_ADD, handle->get_fd(), &ev);

        event_list_.push_back(ev);
        handle->set_index(static_cast<int>(event_list_.size()));
#endif
    }
}

void EventLoop::modify_handle(Handle *handle)
{
    int index = handle->get_index();
    if (index >= 0)
    {
#ifdef Q_OS_MACOS
       short newFilter = handle->getFilter();
       struct kevent& curEvent = event_list_[index];
       EV_SET(&curEvent, curEvent.ident, newFilter, EV_ADD, 0, 0, static_cast<void*>(handle));
#endif
#ifdef Q_OS_LINUX
       struct epoll_event& ev = event_list_[handle->get_index()];
       ev.events = handle->get_events();
       epoll_ctl(ep_fd_, EPOLL_CTL_MOD, handle->get_fd(), &ev);
#endif
    }
}



void EventLoop::delete_handle(Handle *handle)
{
    int index = handle->get_index();
    if (index >= 0)
    {
#ifdef Q_OS_MACOS
        struct kevent& curEvent = event_list_[index];
        EV_SET(&curEvent, curEvent.ident, curEvent.filter, EV_DELETE, 0, 0, static_cast<void*>(handle));
#endif
#ifdef Q_OS_LINUX
        struct epoll_event& ev = event_list_[index];
        if (index != event_list_.size() - 1)
        {
            Handle* back_handle = static_cast<Handle*>(event_list_.back().data.ptr);
            back_handle->set_index(static_cast<int>(event_list_.size()) - 1);
            std::swap(event_list_[index], event_list_.back());
        }

        assert(handle->get_fd() == event_list_.back().data.fd);
        epoll_ctl(ep_fd_, handle->get_fd(), EPOLL_CTL_DEL, nullptr);
#endif
        event_list_.pop_back();
    }
}

bool EventLoop::isInInitThread()
{
    return threadId_ == std::this_thread::get_id();
}


