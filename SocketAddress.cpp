//
// Created by finefenng on 2018/7/5.
//

#include "SocketAddress.h"

#include <arpa/inet.h>
#include <netinet/in.h>


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

SocketAddress::SocketAddress(const char* ip, const int& port)
: ip_(ip), port_(port)
{
    socketAddress_.sin_family = AF_INET;
    socketAddress_.sin_port = ::htons(port);
    ::inet_aton(ip, &socketAddress_.sin_addr);
}

SocketAddress::~SocketAddress()
{

}

sockaddr_in SocketAddress::getSocketAddress()
{
    return socketAddress_;
}

