//
// Created by finefenng on 2018/7/5.
//

#include "Acceptor.h"

#include "Common.h"

#include "EventLoop.h"

#include "Socket.h"

Acceptor::Acceptor(EventLoop* eventLoop, struct sockaddr_in serverAddr)
        : eventLoop_(eventLoop), serverAddr_(serverAddr),
          socket_(new Socket(GetInitIPv4StreamSocketFd())), index_(-1),
          filter_(EVFILT_READ)
{
}

Acceptor::Acceptor(EventLoop* eventLoop, SocketAddress socketAddress)
        : Acceptor(eventLoop, socketAddress.getSocketAddress())
{
}

bool Acceptor::listen()
{
    if (::listen(socket_->getFd(), SOMAXCONN) < 0) {
        return false;
    }

    eventLoop_->registerHandle(this);
    return true;
}

void Acceptor::handleCallbackFunction(struct kevent* event)
{
    int readyQueueLength = static_cast<int>(event->data);
    for (int i = 0; i < readyQueueLength; ++i)
    {
        struct sockaddr_in clientAddr;
        socklen_t len;
        bzero(&clientAddr, 0);

        ::accept(socket_->getFd(), (struct sockaddr*)&clientAddr, &len);



    }


}

int Acceptor::getIndex()
{
    return index_;
}

int Acceptor::setIndex(int index)
{
    index_ = index;
}


int Acceptor::getFd()
{
    return socket_->getFd();
}

short Acceptor::getFilter()
{
    return filter_;

}



