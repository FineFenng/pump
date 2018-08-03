//
// Created by finefenng on 2018/7/8.
//

#include <sys/epoll.h>
#include "TcpConnection.h"

#include "EventLoop.h"
#include "IO_Handle.h"
#include "Socket.h"



TcpConnection::TcpConnection(EventLoop* event_loop, int fd, const SocketAddress &local_address, const SocketAddress &peer_address)
:   event_loop_(event_loop), socket_(new Socket(fd)),
    handle_(new IO_Handle(event_loop, socket_->get_fd())),
    local_address_(local_address), peer_address_(peer_address)
{
    handle_->enable_readable();
    handle_->enable_writable();
    handle_->set_readable_callback(std::bind(&TcpConnection::on_readable_callback, this));
    handle_->set_writable_callback(std::bind(&TcpConnection::on_writable_callback, this));
}

void TcpConnection::on_readable_callback()
{

}

void TcpConnection::on_writable_callback()
{

}

