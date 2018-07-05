//
// Created by finefenng on 2018/7/5.
//

#include "Socket.h"

#include "Common.h"

Socket::Socket(int fd)
:fd_(fd)
{

}

int Socket::getFd() const
{
    return fd_;
}
