//
// Created by finefenng on 2018/7/5.
//

#include <pump/net/Acceptor.h>

#include <cerrno>
#include <cstring>


namespace pump { namespace net
{
Acceptor::Acceptor(EventLoop* event_loop, struct sockaddr_in server_address)
	: event_loop_(event_loop),
	server_address_(server_address),
	socket_(SocketOpen(AF_INET, SOCK_STREAM, 0)),
	watcher_(event_loop_, socket_.get_fd()),
	is_listening_(false)
{
	watcher_.enable_readable();
	watcher_.set_readable_callback(std::bind(&Acceptor::on_new_connection, this));
}

Acceptor::Acceptor(EventLoop* event_loop, SocketAddress socket_address)
	: Acceptor(event_loop, *socket_address.getSocketAddress())
{}

Acceptor::~Acceptor()
{
	if (is_listening_) {
		is_listening_ = false;
		watcher_.disable_all();
	}
}

bool Acceptor::listen()
{
	assert(!is_listening_);
	const SOCKET fd = socket_.get_fd();
	if (SocketBind(fd, server_address_.getSocketAddress()) != pump::kSuccess) {
		LOG_ERROR << " Server bind address failed.";
	}

	if (SocketListen(fd) != pump::kSuccess) {
		LOG_ERROR << "Sever start listening failed.";
	}
	else {
		LOG_INFO << "Sever start listening success.";
	}

	is_listening_ = true;
	return is_listening_;
}


void Acceptor::on_new_connection() const
{
	struct sockaddr_in address{};
	memset(&address, 0, sizeof(address));

	SOCKET conn_fd;

	do {
		conn_fd = SocketAccept(socket_.get_fd(), &address);

		if (!SocketIsValid(conn_fd)) {
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				return;
			}
			else if (errno == EINTR) {
				continue;
			}
			else {
				//TODO ??? how to resolve other errno.
				return;
			}
		}
		else {
			break;
		}
	} while (true);

	SocketAddress client_address(address);

	LOG_DEBUG << "New Connection has been established, and Address [" << client_address.getSocketAddressString().c_str()
 << ']';

	if (new_connection_callback_) {
		new_connection_callback_(conn_fd, client_address);
	}
}

void Acceptor::set_reuse_address_option() const
{
	const SOCKET re = SocketSetReuseAddress(socket_.get_fd());
	(void)re;
}
}}
