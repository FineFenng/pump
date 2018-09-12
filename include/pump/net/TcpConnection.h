//
// Created by finefenng on 2018/7/8.
//

#ifndef QPSTEST_TCPCONNECTION_H
#define QPSTEST_TCPCONNECTION_H

#include <functional>
#include <memory>

#include <pump/net/Buffer.h>
#include <pump/net/Socket.h>
#include <pump/net/SocketAddress.h>
#include <pump/net/watcher/IOWatcher.h>
#include <pump/net/SocketOption.h>
#include <pump/Packet.h>


namespace pump {namespace net
{
class EventLoop;
class Handler;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const std::shared_ptr<TcpConnection>&, Buffer*) MessageReadableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const std::shared_ptr<TcpConnection>&, int, Buffer*) MessageWritableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const std::shared_ptr<TcpConnection>&) ConnectionCloseCallback;

public:
	enum ConnectionStyle
	{
		kDisconnected,
		kEstablishing,
		kConnected,
	};

public:
	TcpConnection(EventLoop* loop, int fd,
				const SocketAddress& local_address, const SocketAddress& peer_address);

PUMP_DECLARE_NON_COPYABLE(TcpConnection)
PUMP_DECLARE_NON_MOVABLE(TcpConnection)

	~TcpConnection();

	void send(const char* data, size_t len);

	void on_readable();

	void on_writable();

	void on_erroneous() const
	{
		const int saved_errno = SocketGetLastErrno(fd_);
		LOG_ERROR << "TcpConnection::handleError - SO_ERROR = "
			<< saved_errno;
	}

	/*
	* below three member function, always called by TcpServer Object
	*/

	void set_readable_callback(const MessageReadableCallback& cb)
	{
		readable_callback_ = cb;
	}

	void set_writable_callback(const MessageWritableCallback& cb)
	{
		writable_callback_ = cb;
	}

	void set_closed_callback(const ConnectionCloseCallback& cb)
	{
		close_callback_ = cb;
	}

	int get_connection_style() const { return state_; }

	void set_connection_style(const ConnectionStyle& state) { state_ = state; }

	void close_connection();

	void send_in_bind_thread(const char* data, size_t len);

	SOCKET get_fd() const { return socket_.get_fd(); }

private:
	EventLoop* loop_;
	Socket socket_;
	SocketAddress local_address_;
	SocketAddress peer_address_;
	IOWatcher handle_;
	mutable int fd_;

private:
	MessageReadableCallback readable_callback_;
	MessageWritableCallback writable_callback_;
	ConnectionCloseCallback close_callback_;

private:
	ConnectionStyle state_;
	Buffer input_buffer_;
	Buffer output_buffer_;
};

typedef std::shared_ptr<TcpConnection> TcpConnection_Ptr;
}}

#endif //QPSTEST_TCPCONNECTION_H
