//
// Created by finefenng on 2018/7/8.
//

#ifndef QPSTEST_TCPCONNECTION_H
#define QPSTEST_TCPCONNECTION_H

#include <memory>
#include <string>

#include "Socket.h
#include "SocketAddress.h"


class EventLoop;
class TcpConnection
{
public:
    TcpConnection(EventLoop* eventLoop, int fd, SocketAddress& localAddr, SocketAddress& peerAddr);

    ~TcpConnection();


    void sendMessage(const void* message, int len);

    void sendMessage(const std::string& message);


private:
    EventLoop* eventLoop_;
    std::unique_ptr<Socket> socket_;
    SocketAddress localAddr_;
    SocketAddress peerAddr_;

};


#endif //QPSTEST_TCPCONNECTION_H
