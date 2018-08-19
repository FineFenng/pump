//
// Created by fenng on 18-8-3.
//

#include <pump/net/TcpServer.h>

#include <cassert>
#include <functional>
#include <pump/net/Acceptor.h>


using namespace std::placeholders;

namespace pump { namespace net
{
    TcpServer::TcpServer(EventLoop* loop, SocketAddress server_address)
        : loop_(loop), server_address_(server_address),
          acceptor_(new Acceptor(loop_, server_address_))
    {
        acceptor_->set_reuseaddr_option();
        acceptor_->set_init_connection_callback(std::bind(&TcpServer::init_connection, this, _1, _2));
        acceptor_->listen();
        LOG_INFO << "Server Started, ####################################################";
    }

    TcpServer::TcpServer(EventLoop* loop, const char* ip, unsigned short port)
        : TcpServer(loop, SocketAddress(ip, port))
    {
    }

    TcpServer::TcpServer(EventLoop* loop, unsigned short port)
        : TcpServer(loop, SocketAddress(port))
    {
    }

    void TcpServer::on_new_collection(const TcpConnection_Ptr& connection_ptr) const
    {
        if (new_connection_callback_) { new_connection_callback_(connection_ptr); }
    }

    void TcpServer::on_message_readable(const TcpConnection_Ptr& connection_ptr, Buffer* buffer) const
    {
        if (message_readable_callback_) { message_readable_callback_(connection_ptr, buffer); }
    }

    void TcpServer::on_message_writable(const TcpConnection_Ptr& connection_ptr, Buffer* buffer) const
    {
        if (message_writable_callback_) { message_writable_callback_(connection_ptr, buffer); }
    }

    void TcpServer::init_connection(int fd, const SocketAddress& client_address)
    {
        //SetSocketNoblocking(fd); ��ʱ��������
        TcpConnection_Ptr new_connection_ptr = std::make_shared<TcpConnection>(loop_, fd, client_address,
                                                                               server_address_);

        new_connection_ptr->set_readable_callback(std::bind(&TcpServer::on_message_readable, this, _1, _2));
        new_connection_ptr->set_writable_callback(std::bind(&TcpServer::on_message_writable, this, _1, _2));
        new_connection_ptr->set_closed_callback(std::bind(&TcpServer::destroy_connection, this, _1));
        new_connection_ptr->set_connection_style(TcpConnection::establishing);

        tcp_connection_map_[fd] = new_connection_ptr;
        on_new_collection(new_connection_ptr);
    }

    void TcpServer::destroy_connection(const TcpConnection_Ptr& connection_ptr)
    {
        connection_ptr->set_connection_style(connection_ptr->disconnected);

        tcp_connection_map_.erase(connection_ptr->get_fd());
    }
}}