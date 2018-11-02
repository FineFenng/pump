//
// Created by finefenng on 2018/7/5.
//

#ifndef PUMP_NET_ACCEPTOR_H_
#define PUMP_NET_ACCEPTOR_H_

#include <functional>

#include <pump/Common.h>
#include <pump/net/watcher/IOWatcher.h>
#include <pump/net/SocketAddress.h>
#include <pump/net/TcpConnection.h>

namespace pump {namespace net {
	class EventLoop;

	class Socket;

	class SocketAddress;

	class TcpConnection;

	class Acceptor {
	PUMP_DECLARE_DELETE_COPYABLE_AND_MOVABLE(Acceptor)
	public:
		PUMP_DECLARE_CALLBACK_FUNCTION(void, int, const SocketAddress&) NewConnectionCallback;

	public:
		Acceptor(EventLoop* event_loop, struct sockaddr_in server_address);

		Acceptor(EventLoop* event_loop, SocketAddress socket_address);

		~Acceptor();

<<<<<<< HEAD

		bool listen();

		void set_reuse_address_option() const;
=======
	bool listen();
>>>>>>> master

		void on_new_connection() const;

		void set_init_connection_callback(const NewConnectionCallback& cb) { new_connection_callback_ = cb; }

	private:
		EventLoop* event_loop_;
		SocketAddress server_address_;
		Socket socket_;

		IOWatcher watcher_;
		bool is_listening_;
	private:
		NewConnectionCallback new_connection_callback_;
	};
}}
#endif
