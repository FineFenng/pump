//
// Created by fenng on 18-8-3.
//

#include "TcpServer.h"

#include <functional>
#include "Acceptor.h"

using namespace std::placeholders;

TcpServer::TcpServer(EventLoop *loop, const char *ip, int port)
: TcpServer(loop, SocketAddress(ip, port))
{

}
TcpServer::TcpServer(EventLoop *loop, SocketAddress server_address)
: loop_(loop), server_address_(server_address),
  acceptor_(new Acceptor(loop_, server_address_))
{
    acceptor_->set_new_connection_callback(std::bind(&TcpServer::on_new_collection, this, _1, _2));
}

void TcpServer::on_new_collection(int fd, const SocketAddress& address)
{
    TcpConnection_Ptr new_connection_ptr = std::make_shared<TcpConnection>(loop_, fd, address, server_address_);
    tcp_connection_map_[fd] = new_connection_ptr;
    if (new_connection_callback_) { new_connection_callback_(new_connection_ptr); }



}



