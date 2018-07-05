//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_ACCEPTOR_H
#define QPSTEST_ACCEPTOR_H

#include <netinet/in.h>
#include <sys/socket.h>


/*  通用套接字结构
 *  struct sockaddr {
 *     uint8_t       sa_len;
 *     sa_family_t   sa_family;
 *     char          sa_data[4];
 *  }
 */


 /*  IPv4地址套接字结构
  *  struct sockaddr_in {
  *     uint8_t         sin_len;
  *     sa_family_t     sin_family;
  *     in_port_t       sin_port;
  *     struct in_addr  sin_addr;
  *  }
  *
  *  struct in_addr {
  *     in_addr_t       s_addr;
  *  }
  */







class EventLoop;

class Acceptor
{
public:
    Acceptor(EventLoop* eventLoop, struct sockaddr_in serverAddr);

private:
    EventLoop* eventLoop_;
    struct socketaddr_in serverAddr_;

};


#endif //QPSTEST_ACCEPTOR_H
