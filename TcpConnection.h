//
// Created by finefenng on 2018/7/8.
//

#ifndef QPSTEST_TCPCONNECTION_H
#define QPSTEST_TCPCONNECTION_H

#include <functional>
#include <memory>
#include <string>

#include "Handle.h"
#include "Socket.h"
#include "SocketAddress.h"
#include "IO_Handle.h"


class EventLoop;
class TcpConnection: public std::enable_shared_from_this<TcpConnection>
{
public:
    typedef std::function<void(std::string& message)> ReadableCallback;
    typedef std::function<void(std::string& message)> WritableCallback;
public:
    TcpConnection(EventLoop *event_loop, int fd, const SocketAddress &local_address, const SocketAddress &peer_address);

    ~TcpConnection();

    void on_readable_callback();

    void on_writable_callback();

    void set_readable_callback(const ReadableCallback& cb) { readable_callback_ = cb; }

    void set_writable_callback(const WritableCallback& cb) { writable_callback_ = cb; }


private:
    EventLoop* event_loop_;
    std::unique_ptr<Socket> socket_;
    SocketAddress local_address_;
    SocketAddress peer_address_;
    int index_;
    unsigned int events_;
    IO_Handle handle_;


private:
    ReadableCallback readable_callback_;
    WritableCallback writable_callback_;
};

typedef std::shared_ptr<TcpConnection> TcpConnection_Ptr;


#endif //QPSTEST_TCPCONNECTION_H
