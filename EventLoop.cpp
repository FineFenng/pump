//
// Created by finefenng on 2018/7/5.
//

#include "EventLoop.h"

#include "Handle.h"
#include "Acceptor.h"

#ifdef Q_OS_LINUX
#include <sys/epoll.h>
#else
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


void EventLoop::run()
{

    int ioHandle;

#ifdef Q_OS_LINUX
    ioHandle = epoll_create();
#else
    ioHandle = kqueue();
    struct kevent* alreadyHandleAddr = nullptr;
    int alreadyHandleNum = kevent(ioHandle, handList_.data(), static_cast<int>(handList_.size()),
                                  alreadyHandleAddr, 0, nullptr);
    if (alreadyHandleNum)
    {
        for (int i = 0; i < alreadyHandleNum; ++i)
        {
            if ( &alreadyHandleAddr[i]  && alreadyHandleAddr[i].udata)
            {
                auto callFunc = static_cast<void(int, int)>(alreadyHandleAddr[i].udata);
                callFunc()
            }


        }


    }
#endif


}

void EventLoop::registerHandleCallBackFunction(Handle* handle)
{
    if (handle->getIndex() < 0)
    {
        struct kevent event;
        uintptr_t ident = static_cast<uintptr_t>(handle->getFd());
        short filter = handle->getFilter();
        auto udata = std::bind(&Acceptor::handleCallbackFunction, handle);
        EV_SET(&event, ident, filter, 0, 0, 0, static_cast<void*>(&udata));
        handList_.push_back(event);
        handle->setIndex(static_cast<int>(handList_.size()));
    }
}

void EventLoop::updateHandleCallBackFunction(Handle* handle)
{
    if (handle->getIndex() >= 0)
    {





    }

}


