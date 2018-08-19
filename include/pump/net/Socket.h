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
		: fd_(fd),
		is_validated_(false)
	{ }

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    Socket(Socket&&) = delete;
    Socket& operator=(Socket&&) = delete;

	void close() const
	{
        int saved_errno = -1;
		if (SocketClose(fd_, &saved_errno) == -1) {
			//TODO How to deal with signal???
			if (saved_errno == EINTR) {
                close();
			}
		}
		is_validated_ = false;
	}


	~Socket()
	{
		if (is_validated_) {
			close();
		}
	}

	SOCKET get_fd() const
	{
		return fd_;
	}

private:
	mutable  SOCKET fd_;
	mutable bool is_validated_;
};
}}

#endif //QPSTEST_SOCKET_H
