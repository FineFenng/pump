//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_ACCEPTOR_H
#define QPSTEST_ACCEPTOR_H

#include <netinet/in.h>
#include <sys/socket.h>

#include "Handle.h"
#include "SocketAddress.h"
#include "TcpConnection.h"

#include <memory>
#include <functional>


class EventLoop;
class Socket;
class SocketAddress;
class TcpConnection;


class Acceptor :
{
public:
    typedef std::function<void(int fd, const SocketAddress&)> NewConnectionCallback;
public:
    Acceptor(EventLoop *event_loop, struct sockaddr_in server_address);

    Acceptor(EventLoop* eventLoop, SocketAddress socketAddress);

    ~Acceptor();

    bool listen();

#ifdef Q_OS_MACOS
    void handle_callback(struct kevent *event) override;
#endif

    void on_new_connection();

    void set_new_connection_callback(const NewConnectionCallback& cb) { new_connection_callback_ = cb; }

private:
    EventLoop* event_loop_;
    SocketAddress server_address_;
    std::unique_ptr<Socket> socket_;
    int index_;
    unsigned int events_;

    std::unique_ptr<Handle> handle_;


 private:
    NewConnectionCallback new_connection_callback_;
};


#endif //QPSTEST_ACCEPTOR_H
