//
// Created by finefenng on 2018/7/5.
//

#ifndef PUMP_NET_SOCKETADDRESS_H_
#define PUMP_NET_SOCKETADDRESS_H_


#include <pump/Common.h>

#include <string>

namespace pump { namespace net
{
class SocketAddress
{
public:
	SocketAddress(const char* ip, unsigned short port);

	explicit SocketAddress(unsigned short port);
	explicit SocketAddress(struct sockaddr_in address);

	std::string getSocketAddressString() const;

	const struct sockaddr_in* getSocketAddress() const;

private:
	struct sockaddr_in socket_address_;
};
}}

#endif
