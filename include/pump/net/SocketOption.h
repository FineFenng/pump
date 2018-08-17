//
// Created by fenng on 18-8-10.
//

#ifndef PUMP_SOCKETOPTION_H
#define PUMP_SOCKETOPTION_H

#include <cassert>
#include <pump/Common.h>
#include <pump/config/Constants.h>
#include <pump/utility/log/Logger.h>


namespace pump { namespace net
{
inline int GetInitIPv4StreamSocketFd()
{
	int fd;
	do {
		fd = ::socket(AF_INET, SOCK_STREAM, 0);
		if (fd < 0) {
			if (errno == ENOBUFS || ENOMEM) {
				continue;
			}
		}
		else {
			break;
		}
	} while (true);
	return fd;
}

inline int GetIpFromINETAddress(const sockaddr_in& address, char* ip, socklen_t len)
{
	if (inet_ntop(AF_INET, &address.sin_addr, ip, len) == nullptr) {
		return pump::Contants::kFail;
	}
	return pump::Contants::kSuccess;
}


inline bool SocketIsValid(SOCKET socket) { return socket >= 0; }

int GetLastSocketError();

inline SOCKET SocketOpen(const int& type)
{
	int protocol = 0;
#ifdef PUMP_PLATFORM_WIN
	if (type == SOCK_STREAM) { protocol = IPPROTO_TCP; }
	if (type == SOCK_DGRAM) { protocol = IPPROTO_UDP; }
#endif

	const SOCKET fd = socket(AF_INET, type, protocol);
	return fd;
}


inline int SocketBind(SOCKET fd, const struct sockaddr_in& peer_address)
{
	if (bind(fd, reinterpret_cast<const struct sockaddr*>(&peer_address), sizeof(peer_address)) == 0) {
		return pump::Contants::kSuccess;
	}
	return pump::Contants::kFail;
}


#ifdef PUMP_PLATFORM_GNU
#define MAX_BACK_LOG SOMAXCONN
#endif

#ifdef PUMP_PLATFORM_WIN
#define MAX_BACK_LOG 256
#endif

inline int SocketListen(SOCKET fd, int backlog = MAX_BACK_LOG)
{
	return listen(fd, backlog) == 0 ? pump::Contants::kSuccess : pump::Contants::kFail;
}

inline SOCKET SocketAccept(SOCKET fd, sockaddr_in* address)
{
	socklen_t len = sizeof(*address);
	const SOCKET conn_fd = accept(fd, reinterpret_cast<struct sockaddr*>(address), &len);
	return SocketIsValid(conn_fd) ? conn_fd : -1;
}

inline int SocketClose(SOCKET fd, int* saved_errno)
{
#if PUMP_PLATFORM_WIN
	if (closesocket(fd) < 0) {
		*saved_errno = errno;
		return -1;
	}
#else
	if (close(fd) < 0) {
		*saved_errno = errno;
		return -1;
	}
#endif
	return 0;
}

inline void SetSocketNoblocking(SOCKET fd)
{
#if PUMP_PLATFORM_WIN
	u_long val = 1;
	ioctlsocket(fd, FIONBIO, &val);
#else
        fcntl(fd, F_SETFL, O_NONBLOCK);
#endif
}

inline int SetSocketReuseAddress(int fd)
{
	char opt_val = 1;
	return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));
}

inline uint32_t SendN(SOCKET fd, const char* const buffer, size_t len, int flags, int* o_errno)
{
	uint32_t re = 0;
	do {
		const int r = ::send(fd, buffer + re, len - re, flags); /*send return value equal -1 or the num wrote */
		if (r > 0) {
			re += r;
			*o_errno = 0;
			if (re == len) {
				break;
			}
		}
		else {
			assert(r == -1);
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				*o_errno = errno;
				break;
			}
			else if (errno == EINTR) {
				continue;
			}
			else {
				LOG_ERROR << fd << " Send failed, error code " << errno;
				*o_errno = errno;
				break;
			}
		}
	} while (true);

	LOG_TRACE << fd << " send; to send:" << len << "; sent:" << re << "; error code " << *o_errno;
	return re;
}

inline uint32_t Send(SOCKET fd, const char* const buffer, size_t len, int flags, int* o_errno)
{
	uint32_t re = 0;
	do {
		const int r = ::send(fd, buffer + re, len - re, flags);
		if (r > 0) {
			re += r;
			*o_errno = 0;
			break;
		}
		else {
			assert( r == -1);
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				*o_errno = errno;
				break;
			}
			else if (errno == EINTR) {
				continue;
			}
			else {
				LOG_ERROR << fd << " Send failed, error code " << errno;
				*o_errno = errno;
				break;
			}
		}
	} while (true);
	LOG_TRACE << fd << " send; to send:" << len << "; sent:" << re << "; error code " << *o_errno;
	return re;
}

inline uint32_t RecvN(SOCKET fd, char* const buffer, size_t len, int flags, int* o_errno)
{
	uint32_t re = 0;
	do {
		const int r = ::recv(fd, buffer + re, len - re, flags);
		if (r > 0) {
			re += r;
			*o_errno = 0;
			if (re == len) {
				break;
			}
		}
		else if (r == 0) {
			LOG_TRACE << fd << " socket closed by remote side gracefully[detected by recv]";
			*o_errno = 0;
			break;
		}
		else {
			assert(r == -1);
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				*o_errno = errno;
				break;
			}
			else if (errno == EINTR) {
				continue;
			}
			else {
				LOG_ERROR << fd << " Recv failed, error code " << errno;
				*o_errno = errno;
				break;
			}
		}
	} while (true);
	LOG_TRACE << fd << " recv; to recv:" << len << "; recvd:" << re << "; error code " << *o_errno;
	return re;
}


inline uint32_t Recv(SOCKET fd, char* const buffer, size_t len, int flags, int* o_errno)
{
	uint32_t re = 0;
	do {
		const int r = ::recv(fd, buffer + re, len - re, flags);
		if (r > 0) {
			re += r;
			*o_errno = 0;
		}
		else if (r == 0) {
			LOG_TRACE << fd << " socket closed by remote side gracefully[detected by recv]";
			*o_errno = 0;
			break;
		}
		else {
			assert(r == -1);
			if (errno == EWOULDBLOCK || errno == EAGAIN) {
				*o_errno = errno;
				break;
			}
			else if (errno == EINTR) {
				continue;
			}
			else {
				LOG_ERROR << fd << " Recv failed, error code " << errno;
				*o_errno = errno;
				break;
			}
		}
	} while (true);
	LOG_TRACE << fd << " recv; to recv:" << len << "; recvd:" << re << "; error code " << *o_errno;
	return re;
}
} //namespace net
} // namespace pump

#endif //PUMP_SOCKETOPTION_H
