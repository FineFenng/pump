//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_ACCEPTOR_H
#define QPSTEST_ACCEPTOR_H

#include <netinet/in.h>
#include <sys/socket.h>

#include "Handle.h"
#include "SocketAddress.h"

#include <memory>
#include <functional>


class EventLoop;

class Socket;

class SocketAddress;

class Acceptor : public Handle
{
public:
    Acceptor(EventLoop* eventLoop, struct sockaddr_in serverAddr);

    Acceptor(EventLoop* eventLoop, SocketAddress socketAddress);

    ~Acceptor();

    bool listen();

    void handleCallbackFunction(int fd, int revents) override;

    int getIndex() override;

    int setIndex(int index) override;

    int getFd() override;

    short getFilter() override;


private:
    EventLoop* eventLoop_;
    struct sockaddr_in serverAddr_;
    std::unique_ptr<Socket> socket_;
    int index_;
    short filter_;
};


#endif //QPSTEST_ACCEPTOR_H
