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
#include <pump/net/watcher/IO_Watcher.h>


namespace pump {namespace net
{
class EventLoop;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::function<void(std::shared_ptr<TcpConnection>, Buffer*)> MessageReadableCallback;
	typedef std::function<void(std::shared_ptr<TcpConnection>, Buffer*)> MessageWritableCallback;
	typedef std::function<void(const std::shared_ptr<TcpConnection>&)> ConnectionCloseCallback;
	typedef std::function<void(const std::shared_ptr<TcpConnection>&)> ConnectionErroneousCallback;

public:
	enum Connection_Style
	{
		disconnected,
		establishing,
		connected,
	};

public:
	TcpConnection(EventLoop* loop, int fd,
				const SocketAddress& local_address, const SocketAddress& peer_address);

	TcpConnection(const TcpConnection&) = delete;
	TcpConnection& operator=(const TcpConnection&) = delete;

	TcpConnection(TcpConnection&&) = delete;
	TcpConnection& operator=(TcpConnection&&) = delete;


	~TcpConnection();

	void on_readable();

	void on_writable();

	void send(const char* data, size_t len);
	void send(const std::string& message);

	void on_erroneous()
	{
		//TODO
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

	void set_erroneous_callback(const ConnectionErroneousCallback& cb) { erroneous_callback_ = cb; }

	int get_connection_style() const { return state_; }

	void set_connection_style(const Connection_Style& state) { state_ = state; }

	void close_connection();

	int get_fd() const { return socket_->get_fd(); }

private:
	EventLoop* loop_;
	std::unique_ptr<Socket> socket_;
	SocketAddress local_address_;
	SocketAddress peer_address_;
	IO_Watcher handle_;
	mutable int fd_;

private:
	MessageReadableCallback readable_callback_;
	MessageWritableCallback writable_callback_;
	ConnectionErroneousCallback erroneous_callback_;
	ConnectionCloseCallback close_callback_;

private:
	Connection_Style state_;

	Buffer input_buffer_;
	Buffer output_buffer_;
};

typedef std::shared_ptr<TcpConnection> TcpConnection_Ptr;
}}

#endif //QPSTEST_TCPCONNECTION_H