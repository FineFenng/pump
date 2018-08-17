//
// Created by finefenng on 2018/7/5.
//

#include <pump/net/EventLoop.h>

#include <cassert>
#include <vector>

#include <pump/net/BackendAbstract.h>
#include <pump/net/WatchAbstract.h>
#include <pump/net/Acceptor.h>

#ifdef PUMP_PLATFORM_GNU
#include <pump/net/backend/Epoll.h>
#include <pump/net/backend/Poll.h>
#include <pump/net/backend/Select.h>
#endif

#ifdef PUMP_PLATFORM_MACX
#include <pump/net/backend/Kqueue.h>
#endif

#ifdef PUMP_PLATFORM_WIN
#include <pump/net/backend/Select.h>
#endif


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

namespace pump {namespace net
{
    static const unsigned int MAX_SIZE = 500;

    thread_local EventLoop* t_eventLoop = nullptr;

    EventLoop::EventLoop()
        : is_looping_(false),
          backend_(),
          threadId_(std::this_thread::get_id())
    {
        if (t_eventLoop == nullptr)
        {
            t_eventLoop = this;
        }

#ifdef PUMP_PLATFORM_WIN
        backend_.reset(new Select(this));
#endif

#ifdef PUMP_PLATFORM_MACX
  backend_.reset(new Kqueue(this));
#endif

#ifdef PUMP_PLATFORM_GNU
  backend_.reset(new Epoll(this));
#endif

        backend_->init_backend();
    }

    EventLoop::~EventLoop()
    {
    }

    void EventLoop::run()
    {
        is_looping_ = true;

        while (is_looping_)
        {
            backend_->poll();
        }
        assert(!is_looping_);
        is_looping_ = false;
    }

    void EventLoop::update_handle(const WatchAbstract& handle) const
    {
        if (handle.get_index() < 0)
        {
            add_handle(handle);
        }
        else
        {
            modify_handle(handle);
        }
    }

    void EventLoop::remove_handle(const WatchAbstract& handle) const
    {
        delete_handle(handle);
    }

    void EventLoop::add_handle(const WatchAbstract& handle) const
    {
        backend_->add_interests(handle);
    }

    void EventLoop::modify_handle(const WatchAbstract& handle) const
    {
        backend_->modify_interests(handle);
    }

    void EventLoop::delete_handle(const WatchAbstract& handle) const
    {
        backend_->delete_interests(handle);
    }

    bool EventLoop::isInInitThread()
    {
        return threadId_ == std::this_thread::get_id();
    }
}}
