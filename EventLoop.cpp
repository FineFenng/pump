//
// Created by finefenng on 2018/7/5.
//

#include "EventLoop.h"

#include "Handle.h"


void EventLoop::registerHandleCallBackFunction(Handle* handle)
{
    if (handle->getIndex() < 0) {
        handList_.push_back(handle->getFd());
        handle->setIndex(static_cast<int>(handList_.size()));
        eventList_[handle->getIndex()] = std::bind(&Handle::handleCallbackFunction, *handle);
    }
}

void EventLoop::updateHandleCallBackFunction(Handle* handle)
{
    if (handle->getIndex() >= 0) {


    }

}
