//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_INETADDR_H
#define QPSTEST_INETADDR_H


#include <netinet/in.h>


class SocketAddress
{
public:
    SocketAddress(const char* ip, const unsigned short& port);

    explicit SocketAddress(struct sockaddr_in addr);

    ~SocketAddress() = default;

    const struct sockaddr_in* getSocketAddress() const;

private:
    struct sockaddr_in socket_address_;
};


#endif //QPSTEST_INETADDR_H
