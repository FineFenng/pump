#ifndef PUMP_NET_ECHOSERVER_H_
#define PUMP_NET_ECHOSERVER_H_

#include <pump/net/TcpServer.h>
#include <pump/net/handle/Hlen.h>


using namespace std::placeholders;
namespace pump { namespace net
{
class EchoServer
{
public:
	typedef std::function<void(const TcpConnection_Ptr&, const std::string&)> CompletePackageCallback;
public:
	EchoServer(EventLoop* loop, SocketAddress socket_address)
		: loop_(loop), server_(loop_, socket_address), parser_(&server_)
	{
		Hlen<uint32_t>::set_complete_package_callback(on_complete_package);
	}

	static void on_complete_package(const TcpConnection_Ptr& tcp_connection_ptr, const std::string& content)
	{
        Hlen<uint32_t>::send(tcp_connection_ptr, content);
	}

private:
	EventLoop* loop_;
	TcpServer server_;
	Hlen<uint32_t> parser_;
};
}}
#endif
