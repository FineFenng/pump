#include <pump/net/SocketOption.h>

namespace pump { namespace net
{
int SocketGetInetStreamPair(const int& type, SOCKET o_sv[2])
{
	if (o_sv == nullptr) {
		return pump::kFail;
	}

	const SOCKET listen_fd = SocketOpen(AF_INET, type, 0);

	if (!SocketIsValid(listen_fd)) {
		return pump::kFail;
	}

	struct sockaddr_in addr_listen, addr_connect, addr_accept;
	::memset(&addr_listen, 0, sizeof(addr_listen));
	socklen_t socklen = sizeof(addr_connect);

	addr_listen.sin_family = AF_INET;
	addr_listen.sin_addr.s_addr = ::htonl(INADDR_LOOPBACK);
	addr_listen.sin_port = 0;

	if (SocketBind(listen_fd, &addr_listen) != pump::kSuccess) {
		SocketClose(listen_fd);
		return pump::kFail;
	}

	if (SocketListen(listen_fd, 1) != pump::kSuccess) {
		SocketClose(listen_fd);
		return pump::kFail;
	}

	if (getsockname(listen_fd, reinterpret_cast<struct sockaddr*>(&addr_connect), &socklen) != pump::kSuccess) {
		SocketClose(listen_fd);
		return pump::kFail;
	}

	const SOCKET conn_fd = SocketOpen(AF_INET, type, 0);

	if (!SocketIsValid(conn_fd)) {
		SocketClose(listen_fd);
		return pump::kFail;
	}

	if (connect(conn_fd, reinterpret_cast<struct sockaddr*>(&addr_connect), sizeof(addr_connect)) != pump::kSuccess) {
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
