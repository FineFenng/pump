//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_ACCEPTOR_H
#define QPSTEST_ACCEPTOR_H


#include <pump/net/watcher/IO_Watcher.h>
#include <pump/net/SocketAddress.h>
#include <pump/net/TcpConnection.h>

#include <memory>
#include <functional>

namespace pump {namespace net
{
class EventLoop;

class Socket;

class SocketAddress;

class TcpConnection;

class Acceptor
{
public:
	typedef std::function<void(int fd, const SocketAddress&)> NewConnectionCallback;
public:
	Acceptor(EventLoop* event_loop, struct sockaddr_in server_address);

	Acceptor(EventLoop* eventLoop, SocketAddress socketAddress);

	~Acceptor();

	Acceptor(const Acceptor&) = delete;
	Acceptor& operator=(const Acceptor&) = delete;

	Acceptor(Acceptor&&) = delete;
	Acceptor& operator=(Acceptor&&) = delete;

	bool listen();

	void set_reuseaddr_option() const;

	void on_new_connection() const;

	void set_init_connection_callback(const NewConnectionCallback& cb) { new_connection_callback_ = cb; }

private:
	EventLoop* event_loop_;
	SocketAddress server_address_;
	std::unique_ptr<Socket> socket_;

	IO_Watcher handle_;
	bool is_listening_;
private:
	NewConnectionCallback new_connection_callback_;
};
}}
#endif //QPSTEST_ACCEPTOR_H
