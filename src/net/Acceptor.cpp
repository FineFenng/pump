//
// Created by finefenng on 2018/7/5.
//

#include <pump/net/Acceptor.h>

#include <cerrno>
#include <cstring>

#include <iostream>

#include <pump/Common.h>
#include <pump/net/EventLoop.h>
#include <pump/net/Socket.h>
#include <pump/net/SocketAddress.h>
#include <pump/net/SocketOption.h>
#include <pump/net/watcher/IO_Watcher.h>
#include <pump/utility/log/Logger.h>

namespace pump { namespace net
{
Acceptor::Acceptor(EventLoop* event_loop, struct sockaddr_in server_address)
	: event_loop_(event_loop), server_address_(server_address),
	socket_(new Socket(SocketOpen(SOCK_STREAM))),
	handle_(event_loop_, socket_->get_fd()),
	is_listening_(false)
{
	handle_.enable_readable();
	handle_.set_readable_callback(std::bind(&Acceptor::on_new_connection, this));
}

Acceptor::Acceptor(EventLoop* eventLoop, SocketAddress socketAddress)
	: Acceptor(eventLoop, *socketAddress.getSocketAddress())
{
}

Acceptor::~Acceptor()
{
	if (is_listening_) {
		is_listening_ = false;
	}
}

bool Acceptor::listen()
{
	const SOCKET fd = socket_->get_fd();
	bind(fd, reinterpret_cast<const struct sockaddr*>(server_address_.getSocketAddress()),
		sizeof(*server_address_.getSocketAddress()));

	if (SocketListen(fd) < 0) {
		LOG_INFO << "Sever start listening failed.";
	}
	else {
		LOG_INFO << "Sever start listening success.";
	}

	is_listening_ = true;
	return is_listening_;
}


void Acceptor::on_new_connection() const
{
	if (!is_listening_) {
		return;
	}

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));

	socklen_t len = sizeof(address);
	const SOCKET connected_fd = accept(socket_->get_fd(), reinterpret_cast<struct sockaddr*>(&address), &len);

	if (connected_fd < 0) {
		const int saved_errno = errno;

		if (saved_errno == EBADF) {
			LOG_FALAL << "Socket fd is not an open file descriptor";
			abort();
		}

		if (saved_errno == ECONNABORTED) {
			LOG_ERROR << "A connection has been aborted";
			return;
		}
	}


	SocketAddress client_address(address);

	LOG_DEBUG << "New Connection has been established, and Address [" << client_address.getSocketAddressString().c_str()
 << ']';

	if (new_connection_callback_) {
		new_connection_callback_(connected_fd, client_address);
	}
}

void Acceptor::set_reuseaddr_option() const
{
	SOCKET re = SetSocketReuseAddress(socket_->get_fd());
}
}}
