//
// Created by finefenng on 2018/7/8.
//

#include <pump/Common.h>
#include <pump/net/TcpConnection.h>

#include <cassert>
#include <string>

#include <pump/net/Buffer.h>
#include <pump/net/EventLoop.h>
#include <pump/net/handle/IO_Handle.h>
#include <pump/net/Socket.h>
#include <pump/net/SocketAddress.h>
#include <pump/net/SocketOption.h>
#include <pump/utility/log/Logger.h>


namespace pump {namespace net
{
TcpConnection::TcpConnection(EventLoop* loop, int fd, const SocketAddress& local_address,
							const SocketAddress& peer_address)
	: loop_(loop),
	socket_(new Socket(fd)),
	handle_(loop, socket_->get_fd()),
	fd_(fd),
	local_address_(local_address),
	peer_address_(peer_address),
	state_(disconnected)

{
	handle_.enable_readable();
	handle_.set_readable_callback(std::bind(&TcpConnection::on_readable, this));
	handle_.set_writable_callback(std::bind(&TcpConnection::on_writable, this));
	handle_.set_erroneous_callback(std::bind(&TcpConnection::on_erroneous, this));
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::on_readable()
{
	const int byte_count = input_buffer_.append_from_fd(socket_->get_fd());

	if (byte_count == 0) {
		LOG_DEBUG << "Peer end has closed the connection";
		close_connection();
	}
	else if (byte_count > 0 && readable_callback_) {
		readable_callback_(shared_from_this(), &input_buffer_);
	}
	else {
		/*count < 0 */
		/*TODO*/
	}
}

void TcpConnection::on_writable()
{
	assert(handle_.is_writable());
	int saved_errno = 0;
	const int wrote_count = Send(get_fd(), output_buffer_.get_readable_address(),
								output_buffer_.get_readable_bytes(), 0, &saved_errno);


	if (saved_errno == 0) {
		output_buffer_.retrieve(static_cast<size_t>(wrote_count));
		if (output_buffer_.get_readable_bytes() == 0) {
			handle_.disable_writable();
		}
	}
	else if (errno == EWOULDBLOCK || errno == EAGAIN) {
		/*TODO*/
	}
	else {
		/* resolve other errors*/
		// TODO -- close connection ???
	}

	if (writable_callback_) { writable_callback_(shared_from_this(), &output_buffer_); }
}

void TcpConnection::send(const char* data, size_t len)
{
	size_t remain_count = len;
	int wrote_count = 0;

	if (!handle_.is_writable() && output_buffer_.get_readable_bytes() == 0) {
		//wrote_count = ::send(get_fd(), data, len, 0);
		int saved_errno = 0;
		wrote_count = Send(get_fd(), data, len, 0, &saved_errno);

		if (saved_errno == 0) {
			remain_count -= wrote_count;
		}
		else if (errno == EWOULDBLOCK || errno == EAGAIN) {
			/*TODO*/

		}
		else {
			//TODO -- close connection ???
		}
	}

	if (remain_count > 0) {

		output_buffer_.append_string(data + wrote_count, remain_count);
		if (!handle_.is_writable()) {
			handle_.enable_writable();
		}
	}
}

void TcpConnection::send(const std::string& message)
{
	send(message.c_str(), message.size());
}


void TcpConnection::close_connection()
{
	handle_.disable_all();
	loop_->remove_handle(handle_);
	close_callback_(shared_from_this());
}
}}
