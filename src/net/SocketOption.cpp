//
// Created by fenng on 18-8-10.
//
#include <pump/net/SocketOption.h>
#include <cassert>
#include <pump/Common.h>
#include <pump/config/Constants.h>
#include <pump/utility/log/Logger.h>


namespace pump { namespace net
{
int GetIpFromINETAddress(const sockaddr_in& address, char* ip, socklen_t len)
{
	if (inet_ntop(AF_INET, &address.sin_addr, ip, len) == nullptr) {
		return pump::kFail;
	}
	return pump::kSuccess;
}


bool SocketIsValid(SOCKET socket)
{
#ifdef PUMP_PLATFORM_WIN
	return socket != INVALID_SOCKET;
#else
	return socket > 0;
#endif
}


SOCKET SocketOpen(int af, int type, int protocol)
{
	int tmp_protocol = protocol;
#ifdef PUMP_PLATFORM_WIN
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { LOG_FALAL << "Windows socket init failed"; }
	if (type == SOCK_STREAM) { tmp_protocol = IPPROTO_TCP; }
	if (type == SOCK_DGRAM) { tmp_protocol = IPPROTO_UDP; }
#endif
	return socket(AF_INET, type, tmp_protocol);
}


int SocketBind(SOCKET fd, const struct sockaddr_in* peer_address)
{
	if (bind(fd, reinterpret_cast<const struct sockaddr*>(peer_address), sizeof(struct sockaddr_in)) == 0) {
		return pump::kSuccess;
	}
	return pump::kFail;
}


#ifdef PUMP_PLATFORM_GNU
#define MAX_BACK_LOG SOMAXCONN
#endif

#ifdef PUMP_PLATFORM_WIN
#define MAX_BACK_LOG 256
#endif

int SocketListen(SOCKET fd, int backlog)
{
	return listen(fd, backlog) == 0 ? pump::kSuccess : pump::kFail;
}

SOCKET SocketAccept(SOCKET fd, struct sockaddr_in* address)
{
	socklen_t len = sizeof(struct sockaddr_in);
	return ::accept(fd, reinterpret_cast<struct sockaddr*>(address), &len);
}

int SocketClose(SOCKET fd)
{
	int re = pump::kSuccess;
#if PUMP_PLATFORM_WIN
	if (closesocket(fd) < 0) {
		re = pump::kFail;
	}
	WSACleanup();
#else
	if (close(fd) < 0) {
		re = pump::kFail;
	}
#endif
	return pump::kSuccess;
}


int SocketGetLastErrno(SOCKET fd)
{
#ifdef PUMP_PLATFORM_WIN
	char optval;
#else
	int optval;
#endif
	auto optlen = static_cast<socklen_t>(sizeof optval);

	if (::getsockopt(fd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0) {
		return errno;
	}
	else {
		return optval;
	}
}


void SocketSetNoblocking(SOCKET fd)
{
#if PUMP_PLATFORM_WIN
	u_long val = 1;
	ioctlsocket(fd, FIONBIO, &val);
#else
	fcntl(fd, F_SETFL, O_NONBLOCK);
#endif
}

int SocketSetReuseAddress(int fd)
{
#ifdef PUMP_PLATFORM_WIN
	char opt_val = 1;
#else
	int opt_val = 1;
#endif
	return setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));
}

/*
* whether in blocking or nonblocking model,
* error num will be set, if errno happend.
*/
int32_t SendN(SOCKET fd, const char* const buffer, size_t len, int flags, int* o_errno)
{
	//TODO

	int32_t re = 0;
	do {

#ifdef PUMP_PLATFORM_WIN
		const int32_t send_count = ::send(fd, buffer + re, static_cast<int>(len) - re, flags);
#else
		const int32_t send_count = ::send(fd, buffer + re, len - re, flags);
#endif
		/*send_in_bind_thread return value equal -1 or the num wrote */
		if (send_count > 0) {
			re += send_count;
			*o_errno = 0;
			if (re == static_cast<int>(len)) {
				break;
			}
		}
		else {
			assert(send_count < 0);
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
	LOG_TRACE << fd << " send_in_bind_thread; to send_in_bind_thread:" << len << "; sent:" << re << "; error code " << *
				o_errno;
	return re;
}

int32_t Send(SOCKET fd, const char* const buffer, size_t len, int flags, int* o_errno)
{
	//TODO

	int32_t re = 0;
	do {
#ifdef PUMP_PLATFORM_WIN
		const int32_t send_count = ::send(fd, buffer + re, static_cast<int>(len) - re, flags);
#else
		const int32_t send_count = ::send(fd, buffer + re, len - re, flags);
#endif

		if (send_count > 0) {
			re += send_count;
			*o_errno = 0;
			break;
		}
		else {
			assert(send_count < 0);
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
	LOG_TRACE << fd << " send_in_bind_thread; to send_in_bind_thread:" << len << "; sent:" << re << "; error code " << *
				o_errno;
	return re;
}

int32_t RecvN(SOCKET fd, char* const buffer, size_t len, int flags, int* o_errno)
{
	int re = 0;
	do {
#ifdef PUMP_PLATFORM_WIN
		const int32_t recv_count = ::recv(fd, buffer + re, static_cast<int>(len) - re, flags);
#else
		const int32_t recv_count = ::recv(fd, buffer + re, len - re, flags);
#endif
		if (recv_count > 0) {
			re += recv_count;
			*o_errno = 0;
			if (re == static_cast<int>(len)) {
				break;
			}
		}
		else if (recv_count == 0) {
			LOG_TRACE << fd << " socket closed by remote side gracefully[detected by recv]";
			*o_errno = 0;
			break;
		}
		else {
			assert(recv_count == -1);
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

int Recv(SOCKET fd, char* const buffer, size_t len, int flags, int* o_errno)
{
	int re = 0;
	do {
#ifdef PUMP_PLATFORM_WIN
		const int32_t recv_count = ::recv(fd, buffer + re, static_cast<int>(len) - re, flags);
#else
		const int32_t recv_count = ::recv(fd, buffer + re, len - re, flags);
#endif
		if (recv_count > 0) {
			re += recv_count;
			*o_errno = 0;
			break;
		}
		else if (recv_count == 0) {
			LOG_TRACE << fd << " socket closed by remote side gracefully[detected by recv]";
			*o_errno = 0;
			break;
		}
		else {
			assert(recv_count < 0);
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


int SocketGetInetStreamPair(const int& type, SOCKET o_sv[2])
{
	if (o_sv == nullptr) {
		return pump::kFail;
	}

	const SOCKET listen_fd = SocketOpen(AF_INET, type, 0);

	if (!SocketIsValid(listen_fd)) {
		return pump::kFail;
	}

	struct sockaddr_in addr_listen{};
	struct sockaddr_in addr_connect{};
	struct sockaddr_in addr_accept{};
	::memset(&addr_listen, 0, sizeof(addr_listen));
	socklen_t socklen = sizeof(struct sockaddr_in);

	addr_listen.sin_family = AF_INET;
	addr_listen.sin_port = 0;
	addr_listen.sin_addr.s_addr = ::htonl(INADDR_LOOPBACK);

	if (SocketBind(listen_fd, &addr_listen) != pump::kSuccess) {
		SocketClose(listen_fd);
		return pump::kFail;
	}

	if (SocketListen(listen_fd, 1) != pump::kSuccess) {
		SocketClose(listen_fd);
		return pump::kFail;
	}

	if (getsockname(listen_fd, reinterpret_cast<struct sockaddr*>(&addr_connect), &socklen) != pump::
		kSuccess) {
		SocketClose(listen_fd);
		return pump::kFail;
	}

	const SOCKET conn_fd = SocketOpen(AF_INET, type, 0);

	if (!SocketIsValid(conn_fd)) {
		SocketClose(listen_fd);
		return pump::kFail;
	}

	if (connect(conn_fd, reinterpret_cast<struct sockaddr*>(&addr_connect), sizeof(addr_connect)) != pump::
		kSuccess) {
		SocketClose(listen_fd);
		SocketClose(conn_fd);
		return pump::kFail;
	}

	const SOCKET accept_fd = accept(listen_fd, reinterpret_cast<struct sockaddr*>(&addr_accept), &socklen);

	if (!SocketIsValid(accept_fd)) {
		SocketClose(conn_fd);
		SocketClose(listen_fd);
		return pump::kFail;
	}

	o_sv[0] = accept_fd;
	o_sv[1] = conn_fd;

	return pump::kSuccess;
}
}}
