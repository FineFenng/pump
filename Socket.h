//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_SOCKET_H
#define QPSTEST_SOCKET_H

#include <boost/noncopyable.hpp>


class Socket
{
public:
    Socket(int fd);
    ~Socket()
    {
        ::close(fd_);
    }

    int getFd() const;

private:
    int fd_;
};


#endif //QPSTEST_SOCKET_H
