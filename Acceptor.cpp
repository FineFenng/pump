//
// Created by finefenng on 2018/7/5.
//

#include "Acceptor.h"

#include "Common.h"

#include "EventLoop.h"

#include "Socket.h"
#include "Platform.h"

Acceptor::Acceptor(EventLoop* eventLoop, struct sockaddr_in serverAddr)
        : eventLoop_(eventLoop), serverAddr_(serverAddr),
          socket_(new Socket(GetInitIPv4StreamSocketFd())), index_(-1)
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
    return true;
}

void Acceptor::handleCallbackFunction(int fd, int revents)
{
#ifdef Q_OS_LINUX
    if (revents  |  )

#endif

#ifdef Q_OS_MAC
    if (revents | ) {

    }

#endif


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

int Acceptor::getEvents()
{
    return events_;
}



