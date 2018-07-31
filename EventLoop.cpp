//
// Created by finefenng on 2018/7/5.
//

#include "EventLoop.h"

#include "Handle.h"
#include "Acceptor.h"


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


thread_local EventLoop* t_eventLoop = nullptr;

EventLoop::EventLoop()
        : handleList_(), alreadyHandList_(),
          isLooping_(false), isQuit_(true),
          threadId_(std::this_thread::get_id())
{
    if (t_eventLoop == nullptr)
    {
        t_eventLoop = this;
    }

}

void EventLoop::run()
{

    int ioHandle = kqueue();
    isLooping_ = true;
    isQuit_ = false;

    while (!isQuit_)
    {
        struct kevent* alreadyHandleAddr = nullptr;
        int alreadyHandleNum = kevent(ioHandle, handleList_.data(), static_cast<int>(handleList_.size()),
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
    isLooping_ = false;
}

void EventLoop::registerHandle(Handle* handle)
{
    if (handle->getIndex() < 0)
    {
        struct kevent event;
        uintptr_t ident = static_cast<uintptr_t>(handle->getFd());
        short filter = handle->getFilter();
        EV_SET(&event, ident, filter, EV_ENABLE, 0, 0, static_cast<void*>(handle));
        handleList_.push_back(event);
        handle->setIndex(static_cast<int>(handleList_.size()));
    }
}

void EventLoop::updateHandle(Handle* handle)
{
    int index = handle->getIndex();
    if (index >= 0)
    {
        struct kevent& curEvent = handleList_[index];
        short newFilter = handle->getFilter();
        EV_SET(&curEvent, curEvent.ident, newFilter, EV_ADD, 0, 0, static_cast<void*>(handle));
    }

}

void EventLoop::disableHandle(Handle* handle)
{
    int index = handle->getIndex();
    if (index >= 0)
    {
        struct kevent& curEvent = handleList_[index];
        EV_SET(&curEvent, curEvent.ident, curEvent.filter, EV_DISABLE, 0, 0, static_cast<void*>(handle));
    }

}


void EventLoop::deleteHandle(Handle* handle)
{
    int index = handle->getIndex();
    if (index >= 0)
    {
        struct kevent& curEvent = handleList_[index];
        EV_SET(&curEvent, curEvent.ident, curEvent.filter, EV_DELETE, 0, 0, static_cast<void*>(handle));
    }
}

bool EventLoop::isInInitThread()
{
    return threadId_ == std::this_thread::get_id();
}


