//
// Created by finefenng on 2018/7/5.
//

#ifndef PUMP_NET_SOCKET_H_
#define PUMP_NET_SOCKET_H_

#include <cerrno>
#include <cstring>

#include <pump/net/SocketOption.h>
#include <pump/Common.h>

namespace pump { namespace net {
class Socket {
 PUMP_DECLARE_DELETE_COPYABLE_AND_MOVABLE(Socket)
 public:
  explicit Socket(SOCKET fd)
	  : fd_(fd) {}


  ~Socket() {
	do {
	  if (SocketClose(fd_) == pump::kFail) {
		if (errno == EWOULDBLOCK || errno == EAGAIN) {
		  continue;
		} else {
		  LOG_ERROR << "Close Socket: " << fd_ << " failed.";
		  return;
		}
	  } else {
		break;
	  }
	} while (true);
  }

  SOCKET get_fd() const {
	return fd_;
  }

 private:
  SOCKET fd_;
};
}}

#endif
