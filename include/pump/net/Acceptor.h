//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_ACCEPTOR_H
#define QPSTEST_ACCEPTOR_H

#include <functional>

#include <pump/Common.h>
#include <pump/net/watcher/IO_Watcher.h>
#include <pump/net/SocketAddress.h>
#include <pump/net/TcpConnection.h>

namespace pump {namespace net
{
class EventLoop;

class Socket;

class SocketAddress;

class TcpConnection;

class Acceptor
{
public:
	PUMP_DECLAE_CALLBACK_FUNCTION(void, int, const SocketAddress&) NewConnectionCallback;

public:
	Acceptor(EventLoop* event_loop, struct sockaddr_in server_address);

	Acceptor(EventLoop* eventLoop, SocketAddress socketAddress);

	~Acceptor();

PUMP_DECLARE_NONCOPYABLE(Acceptor)

PUMP_DECLARE_DEFAULTMOVABLE(Acceptor)

	bool listen();

	void set_reuse_address_option() const;

	void on_new_connection() const;

	void set_init_connection_callback(const NewConnectionCallback& cb) { new_connection_callback_ = cb; }

private:
	EventLoop* event_loop_;
	SocketAddress server_address_;
	Socket socket_;

	IO_Watcher watcher_;
	bool is_listening_;
private:
	NewConnectionCallback new_connection_callback_;
};
}}
#endif //QPSTEST_ACCEPTOR_H
