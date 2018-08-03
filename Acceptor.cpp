//
// Created by finefenng on 2018/7/5.
//

#include "Acceptor.h"

#include <cstring>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "Common.h"
#include "EventLoop.h"
#include "IO_Handle.h"
#include "Socket.h"
#include "SocketAddress.h"
#include "TcpConnection.h"

Acceptor::Acceptor(EventLoop *event_loop, struct sockaddr_in server_address)
        : event_loop_(event_loop), server_address_(server_address),
          socket_(new Socket(GetInitIPv4StreamSocketFd())),
          handle_(new IO_Handle(event_loop_, socket_->get_fd())),
          new_connection_callback_()
{
    handle_->enable_readable();
    handle_->set_readable_callback(std::bind(&Acceptor::on_new_connection, this));
}

Acceptor::Acceptor(EventLoop* eventLoop, SocketAddress socketAddress)
        : Acceptor(eventLoop, *socketAddress.getSocketAddress())
{
}

bool Acceptor::listen()
{
    ::bind(get_fd(), (struct sockaddr*)server_address_.getSocketAddress(), sizeof(*server_address_.getSocketAddress()));

    if (::listen(socket_->get_fd(), SOMAXCONN) < 0) {
        return false;
    }

    event_loop_->add_handle(this);
    return true;
}


#ifdef Q_OS_MACOS
void Acceptor::handle_callback(struct kevent *event)
{
    int readyQueueLength = static_cast<int>(event->data);
    for (int i = 0; i < readyQueueLength; ++i)
    {
        struct sockaddr_in clientAddr;
        socklen_t len;
        bzero(&clientAddr, 0);

        ::accept(socket_->get_fd(), (struct sockaddr*)&clientAddr, &len);
    }
}
#endif

#ifdef Q_OS_LINUX

void Acceptor::on_new_connection()
{
    struct sockaddr_in address;
    ::bzero(&address, 0);
    socklen_t len;
    int connected_fd = ::accept(socket_->get_fd(), (struct sockaddr*)&address, &len);
    SocketAddress client_address(address);
    TcpConnection new_connection(event_loop_, connected_fd, client_address, server_address_);
    if (new_connection_callback_) { new_connection_callback_(new_connection); }
}

#endif







