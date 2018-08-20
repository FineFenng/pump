//
// Created by finefenng on 2018/7/5.
//

#include <pump/net/SocketAddress.h>

#include <pump/Common.h>
#include <cstring>


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

namespace pump {namespace net
{
SocketAddress::SocketAddress(const char* ip, unsigned short port)
{
	socket_address_.sin_family = AF_INET;
	::inet_pton(AF_INET, ip, &socket_address_.sin_addr);
	socket_address_.sin_port = htons(port);
}

SocketAddress::SocketAddress(unsigned short port)
{
	socket_address_.sin_family = AF_INET;
	socket_address_.sin_addr.s_addr = htonl(INADDR_ANY);
	socket_address_.sin_port = htons(port);
}

SocketAddress::SocketAddress(struct sockaddr_in address)
	: socket_address_(address)
{
}

const sockaddr_in* SocketAddress::getSocketAddress() const
{
	return &socket_address_;
}

std::string SocketAddress::getSocketAddressString() const
{
	char address_info[64] = {0};

	::inet_ntop(AF_INET, &socket_address_.sin_addr, address_info, 64);
	uint16_t host_port = ntohs(socket_address_.sin_port);
	size_t ip_len = strlen(address_info);
	snprintf(address_info + ip_len, 64 - ip_len, ":%d", host_port);

	return std::string(address_info, address_info + strlen(address_info));
}
}}
