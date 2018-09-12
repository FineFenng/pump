//
// Created by finefenng on 2018/7/8.
//

#include <pump/Common.h>
#include <pump/net/TcpConnection.h>

#include <cassert>
#include <string>

#include <pump/net/Buffer.h>
#include <pump/net/EventLoop.h>
#include <pump/net/watcher/IOWatcher.h>
#include <pump/net/Socket.h>
#include <pump/net/SocketAddress.h>
#include <pump/net/SocketOption.h>
#include <pump/net/handler/Hlen.h>

#include <pump/utility/log/Logger.h>

namespace pump {namespace net
{
TcpConnection::TcpConnection(EventLoop* loop, int fd, const SocketAddress& local_address,
							const SocketAddress& peer_address)
	: loop_(loop),
	socket_(fd),
	local_address_(local_address),
	peer_address_(peer_address),
	handle_(loop, socket_.get_fd()),
	fd_(fd),
	state_(kDisconnected)

{
	handle_.set_readable_callback(std::bind(&TcpConnection::on_readable, this));
	handle_.set_writable_callback(std::bind(&TcpConnection::on_writable, this));
	handle_.set_erroneous_callback(std::bind(&TcpConnection::on_erroneous, this));
	handle_.enable_readable();
}

TcpConnection::~TcpConnection()
{
	assert(state_ == kDisconnected);
}

// NORMAL_TASK
void TcpConnection::send(const char* data, size_t len)
{
	loop_->push_back_task(std::bind(&TcpConnection::send_in_bind_thread, this, data, len));
}

// IO_TASK
void TcpConnection::on_readable()
{
	int saved_errno;
	const int byte_count = input_buffer_.recv_from_fd(socket_.get_fd(), &saved_errno);

	if (byte_count == 0) {
		LOG_DEBUG << "Peer end has closed the connection";
		close_connection();
	}
	else if (byte_count > 0 && readable_callback_) {
		readable_callback_(shared_from_this(), &input_buffer_);
	}
	else if (byte_count < 0 && (saved_errno != EWOULDBLOCK || saved_errno != EAGAIN)) {
		//TODO
		LOG_ERROR << "Read Sock " << socket_.get_fd() << " error.";
		on_erroneous();
	}
	else { }
}

// IO_TASK
void TcpConnection::on_writable()
{
	assert(handle_.is_writable());
	int saved_errno = 0;
	int wrote_count;
	do {
		wrote_count = Send(get_fd(), reinterpret_cast<char*>(output_buffer_.get_readable_address()),
							output_buffer_.get_readable_bytes(), 0, &saved_errno);

		if (wrote_count > 0) {
			output_buffer_.retrieve(static_cast<size_t>(wrote_count));
			if (output_buffer_.get_readable_bytes() == 0) {
				handle_.disable_writable();
			}
			break;
		}
		else if (errno == EWOULDBLOCK || errno == EAGAIN) {
			// when socket is nonblocking.
			LOG_DEBUG << "Can't write socket:" << get_fd() << ".";
			break;
		}
		else if (errno == EINTR) {
			continue;
		}
		else {
			on_erroneous();
			break;
		}
	} while (true);
	if (writable_callback_) {
		writable_callback_(shared_from_this(), wrote_count, & output_buffer_);
	}
}

void TcpConnection::send_in_bind_thread(const char* data, size_t len)
{
	
	size_t remain_count = len;
	int wrote_count = 0;

	if (!handle_.is_writable() && output_buffer_.get_readable_bytes() == 0) {
		//wrote_count = ::send_in_bind_thread(get_fd(), data, len, 0);
		do {
			int saved_errno = 0;
			wrote_count = Send(get_fd(), data, len, 0, &saved_errno);

			if (wrote_count > 0) {
				remain_count -= wrote_count;
				break;
			}
			else if (errno == EWOULDBLOCK || errno == EAGAIN) {
				// when socket is nonblocking.
				LOG_DEBUG << "Can't write socket:" << get_fd() << ".";
				break;
			}
			else if (errno == EINTR) {
				continue;
			}
			else {
				// It happend probabyly that client port net has been terminated .
				on_erroneous();
				close_connection();
				return;
			}
		} while (true);
	}

	if (remain_count > 0) {
		output_buffer_.append_string(data + wrote_count, remain_count);
		if (!handle_.is_writable()) {
			handle_.enable_writable();
		}
	}
}


void TcpConnection::close_connection()
{
	//handle_.disable_all();
	loop_->remove_watcher_sync(handle_);
	close_callback_(shared_from_this());
}
}}
