//
// Created by finefenng on 2018/7/5.
//


#ifndef PUMP_CONFIG_PLATFORM_H_
#define PUMP_CONFIG_PLATFORM_H_


#if defined(WIN32) || defined(_WIN64)
#define PUMP_PLATFORM_WIN      1
#define PUMP_PLATFORM          PUMP_PLATFORM_WIN
#endif

#ifdef __GNUC__
#define PUMP_PLATFORM_GNU      1
#define PUMP_PLATFORM          PUMP_PLATFORM_GNU
#endif

#if defined(__GNUC__) && defined(__LINUX) || defined(__LINUX__) 
#define PUMP_PLATFORM_LINUX      1
#define PUMP_PLATFORM          PUMP_PLATFORM_LINUX
#endif

#if defined(__APPLE__) && defined(__GNUC__) || defined(__CLANG__)

#define PUMP_PLATFORM_MACX     1
#define PUMP_PLATFORM      PUMP_PLATFORM_MACX
#endif

#ifndef PUMP_PLATFORM
#error
#endif
#ifdef PUMP_PLATFORM_GNU
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <errno.h>

    #include <netinet/tcp.h>
    #include <sys/socket.h>
    #include <fcntl.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <sys/un.h> //sockaddr_un
    #include <sys/uio.h>
    #include <sys/ioctl.h>
	#include <sys/select.h>
	#include <sys/poll.h>
#endif


#ifdef PUMP_PLATFORM_LIUNX
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <errno.h>
    #include <netinet/tcp.h>
    #include <sys/socket.h>
    #include <fcntl.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <sys/un.h> //sockaddr_un
    #include <sys/uio.h>
    #include <sys/ioctl.h>
    #include <sys/epoll.h>
#endif


#ifdef PUMP_PLATFORM_MACX
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <errno.h>
    #include <netinet/tcp.h>
    #include <sys/socket.h>
    #include <fcntl.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <arpa/inet.h>
    #include <sys/un.h> //sockaddr_un
    #include <sys/uio.h>
 	#include <sys/select.h>
	#include <sys/poll.h>
    #include <sys/event.h>
#endif

#ifdef PUMP_PLATFORM_WIN
#include <SDKDDKVer.h>
#include <winsock2.h> //WIN32_LEAN_AND_MEAN will exclude this from Windows.h
#include <WS2tcpip.h> //for inet_ntop, etc
#include <mstcpip.h>
#include <Windows.h> //will include winsock.h
#include <tchar.h>
#include <direct.h>
#include <io.h>
#include <malloc.h>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#endif

#endif //PUMP_CONFIG_PLATFORM_H_
