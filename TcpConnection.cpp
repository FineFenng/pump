//
// Created by finefenng on 2018/7/8.
//

#include "TcpConnection.h"
#include "EventLoop.h"
#include "Socket.h"



TcpConnection::TcpConnection(EventLoop* eventLoop, int fd, SocketAddress& localAddr, SocketAddress& peerAddr)
: eventLoop_(eventLoop), socket_(new Socket(fd)), localAddr_(localAddr), peerAddr_(peerAddr)
{

}
