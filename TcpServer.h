//
// Created by fenng on 18-8-3.
//

#ifndef QPSTEST_TCPSERVER_H
#define QPSTEST_TCPSERVER_H

#include <map>

#include "TcpConnection.h"


class Acceptor;
class TcpServer
{
public:
    typedef std::function<const TcpConnection_Ptr&> NewConnectionPtr;
    typedef std::function<void(const TcpConnection_Ptr&)> NewConnectionCallback;

public:
    TcpServer(EventLoop* loop, const char* ip, int port);
    TcpServer(EventLoop* loop, SocketAddress server_address);

    void add_into_map(int fd, const TcpConnection_Ptr& tcp_connection_ptr) { tcp_connection_map_[fd] = tcp_connection_ptr; }

    void on_new_collection(int fd, const SocketAddress& address);

    void set_new_connection_callback(const NewConnectionCallback& cb) { new_connection_callback_ = cb; }




public:
private:
    EventLoop* loop_;
    SocketAddress server_address_;

private:
    std:: map<int, TcpConnection_Ptr> tcp_connection_map_;
    std::unique_ptr<Acceptor> acceptor_;

private:
    NewConnectionCallback new_connection_callback_;

};


#endif //QPSTEST_TCPSERVER_H
