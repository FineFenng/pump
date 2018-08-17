//
// Created by finefenng on 2018/7/5.
//


#ifndef PUMP_CONFIG_PLATFORM_H_
#define PUMP_CONFIG_PLATFORM_H_

#ifdef WIN32 || _WIN64
#define PUMP_PLATFORM_WIN      1
#define PUMP_PLATFORM          PUMP_PLATFORM_WIN
#elif defined(__GNUC__)
    #define PUMP_PLATFORM_GNU      1
    #define PUMP_PLATFORM          PUMP_PLATFORM_GNU
#elif defined(__APPLE__ && (__GNUC__ || __CLANG__))
    #define PUMP_PLATFORM_MACX     1
    #define PUMP_PLATFORM_MACX      PUMP_PLATFORM_MACX
#else
    #error
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
    #include <sys/epoll.h>
#endif


#endif //PUMP_CONFIG_PLATFORM_H_
