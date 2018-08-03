//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_SOCKET_H
#define QPSTEST_SOCKET_H

#include <boost/noncopyable.hpp>
#include <boost/functional.hpp>


class Socket: public boost::noncopyable
{
public:
    Socket(int fd);
    ~Socket()
    {
        ::close(fd_);
    }

    int get_fd() const;

private:
    int fd_;
};


#endif //QPSTEST_SOCKET_H
