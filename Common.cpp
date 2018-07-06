//
// Created by finefenng on 2018/7/5.
//

#include "Common.h"

#include <sys/socket.h>

int GetInitIPv4StreamSocketFd()
{
    int fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        //TODO
    }

    return fd;
}
