//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_SOCKET_H
#define QPSTEST_SOCKET_H

#include <cerrno>
#include <cstring>

#include <pump/net/SocketOption.h>
#include <pump/Common.h>

namespace pump { namespace net
{
class Socket
{
public:
	explicit Socket(SOCKET fd)
		: fd_(fd)
	{ }

PUMP_DECLARE_NONCOPYABLE(Socket)
PUMP_DECLARE_NONMOVABLE(Socket)


	~Socket()
	{
		int saved_errno;
		do {
			if (SocketClose(fd_, &saved_errno) < 0) {
				if (saved_errno == EWOULDBLOCK || errno == EAGAIN) {
					continue;
				}
				else {
					LOG_ERROR << "Close Socket: " << fd_ << " failed.";
					return;
				}
			}
		} while (true);
	}

	SOCKET get_fd() const
	{
		return fd_;
	}

private:
	SOCKET fd_;
};
}}

#endif //QPSTEST_SOCKET_H
