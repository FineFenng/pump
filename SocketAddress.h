//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_INETADDR_H
#define QPSTEST_INETADDR_H


#include <netinet/in.h>


class SocketAddress
{
public:
    SocketAddress(const char* ip, const int& port);

    ~SocketAddress();

    struct sockaddr_in getSocketAddress();

private:
    const char* ip_;
    const int port_;
    struct sockaddr_in socketAddress_;

};


#endif //QPSTEST_INETADDR_H
