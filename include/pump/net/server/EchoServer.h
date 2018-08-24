#ifndef PUMP_NET_ECHOSERVER_H_
#define PUMP_NET_ECHOSERVER_H_

#include <pump/net/TcpServer.h>
#include <pump/net/handle/Hlen.h>
#include <pump/Packet.h>


using namespace std::placeholders;
namespace pump { namespace net
{
class EchoServer
{
public:

	PUMP_DECLAE_CALLBACK_FUNCTION(void, TcpConnection_Ptr&, const std::string&) CompletePackageCallback;

public:
	EchoServer(EventLoop* loop, SocketAddress socket_address)
		: loop_(loop), server_(loop_, socket_address, 3), handler_(&server_)
	{
		handler_.set_complete_package_callback(std::bind(&EchoServer::on_complete_package, this, _1, _2));
		server_.set_message_readable_callback(std::bind(&Hlen<uint32_t>::on_message, &handler_, _1, _2));
	}

	void on_complete_package(const TcpConnection_Ptr& tcp_connection_ptr, Packet* packet)
	{
		BYTE_T temp[1024] = {0};
        const uint32_t read_num = packet->peek(1024, temp);
        handler_.send(tcp_connection_ptr, temp, read_num);
	}

private:
	EventLoop* loop_;
	TcpServer server_;
	Hlen<uint32_t> handler_;
};
}}
#endif
