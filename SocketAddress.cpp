//
// Created by finefenng on 2018/7/5.
//

#include "SocketAddress.h"

#include <arpa/inet.h>


/*  通用套接字结构
 *  struct sockaddr {
 *     uint8_t       sa_len;
 *     sa_family_t   sa_family;
 *     char          sa_data[4];
 *  }
 */


/*  IPv4地址套接字结构
 *  struct sockaddr_in {
 *     uint8_t         sin_len;
 *     sa_family_t     sin_family;
 *     in_port_t       sin_port;
 *     struct in_addr  sin_addr;
 *  }
 *
 *  struct in_addr {
 *     in_addr_t       s_addr;
 *  }
 */

SocketAddress::SocketAddress(const char* ip, const unsigned short& port)
{
    socket_address_.sin_family = AF_INET;
    socket_address_.sin_port = ::htons(port);
    ::inet_pton(AF_INET, ip, &socket_address_.sin_addr);
}

SocketAddress::SocketAddress(struct sockaddr_in addr)
        : socket_address_(addr)
{

}


const sockaddr_in* SocketAddress::getSocketAddress() const
{
    return &socket_address_;
}

