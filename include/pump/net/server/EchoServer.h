#ifndef PUMP_NET_ECHOSERVER_H
#define PUMP_NET_ECHOSERVER_H

#include <pump/net/TcpServer.h>
#include <pump/Packet.h>
#include <pump/net/handler/Hlen.h>

using namespace std::placeholders;
namespace pump { namespace net
{
class EchoServer
{
public:

	PUMP_DECLARE_CALLBACK_FUNCTION(void, TcpConnection_Ptr&, const std::string&) CompletePackageCallback;

public:
	EchoServer(EventLoop* loop, SocketAddress socket_address)
		: loop_(loop),
		server_(loop_, socket_address, 2)
	{
		server_.set_new_connection_callback(std::bind(&EchoServer::on_new_connection, this, _1));
	}

	void on_new_connection(const TcpConnection_Ptr& tcp_connection_ptr)
	{
		Handler* handler = new Hlen<uint32_t>(&server_, tcp_connection_ptr);
		handler->set_complete_package_callback(std::bind(&EchoServer::on_complete_package, this, _1, _2));
		tcp_connection_ptr->set_handler(handler);
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
};
}}
#endif
