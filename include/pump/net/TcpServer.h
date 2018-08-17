//
// Created by fenng on 18-8-3.
//

#ifndef PUMP_TCPSERVER_H
#define PUMP_TCPSERVER_H

#include <map>
#include <pump/net/Buffer.h>
#include <pump/net/TcpConnection.h>
#include <pump/net/Acceptor.h>

namespace pump
{
namespace net
{

class TcpServer
{
 public:

  typedef std::function<void(const TcpConnection_Ptr &)> NewConnectionCallback;
  typedef std::function<void(const TcpConnection_Ptr &, Buffer *)> MessageReadableCallback;
  typedef std::function<void(const TcpConnection_Ptr &, Buffer *)> MessageWritableCallback;

 public:
  TcpServer(EventLoop *loop, SocketAddress server_address);

  TcpServer(EventLoop *loop, const char *ip, unsigned short port);

  TcpServer(EventLoop *loop, unsigned short port);

  void on_new_collection(const TcpConnection_Ptr &connection_ptr) const;

  void on_message_readable(const TcpConnection_Ptr &connection_ptr, Buffer *buffer) const;

  void on_message_writable(const TcpConnection_Ptr &connection_ptr, Buffer *buffer) const;

  void set_new_connection_callback(const NewConnectionCallback &cb) 
  { new_connection_callback_ = cb; }

  void set_message_readable_callback(const MessageReadableCallback &cb)
  { message_readable_callback_ = cb; }

  void set_message_writable_callback(const MessageWritableCallback &cb)
  { message_writable_callback_ = cb; }

  void init_connection(int fd, const SocketAddress &client_address);

  void destroy_connection(const TcpConnection_Ptr &connection_ptr);

 private:
  EventLoop *loop_;
  SocketAddress server_address_;

 private:
  std::map<int, TcpConnection_Ptr> tcp_connection_map_;
  std::unique_ptr<Acceptor> acceptor_;

 private:
  NewConnectionCallback new_connection_callback_;
  MessageReadableCallback message_readable_callback_;
  MessageWritableCallback message_writable_callback_;
};

}
}

#endif //PUMP_TCPSERVER_H
