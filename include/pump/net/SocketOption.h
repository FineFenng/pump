//
// Created by fenng on 18-8-10.
//

#ifndef PUMP_NET_SOCKETOPTION_H_
#define PUMP_NET_SOCKETOPTION_H_

#include <pump/Common.h>
#include <pump/utility/log/LogLine.h>


namespace pump { namespace net
{
int GetIpFromINETAddress(const sockaddr_in& address, char* ip, socklen_t len);

bool SocketIsValid(SOCKET socket);

SOCKET SocketOpen(int af, int type, int protocol);

int SocketBind(SOCKET fd, const struct sockaddr_in* peer_address);


#ifdef PUMP_PLATFORM_WIN
    #define MAX_BACK_LOG 256
#else
    #define MAX_BACK_LOG  SOMAXCONN
#endif


int SocketListen(SOCKET fd, int backlog = MAX_BACK_LOG);

SOCKET SocketAccept(SOCKET fd, struct sockaddr_in* address);


int SocketClose(SOCKET fd);

int SocketGetInetStreamPair(const int& type, SOCKET o_sv[2]);

int SocketGetLastErrno(SOCKET fd);

void SocketSetNoblocking(SOCKET fd);

int SocketSetReuseAddress(int fd);
/* 
 * whether in blocking or nonblocking model, 
 * error num will be set, if errno happend.
 */
int32_t SendN(SOCKET fd, const char* const buffer, size_t len, int flags, int* o_errno);

int32_t Send(SOCKET fd, const char* const buffer, size_t len, int flags, int* o_errno);

int32_t RecvN(SOCKET fd, char* const buffer, size_t len, int flags, int* o_errno);

int Recv(SOCKET fd, char* const buffer, size_t len, int flags, int* o_errno);
} //namespace net
} // namespace pump

#endif //PUMP_SOCKETOPTION_H_
