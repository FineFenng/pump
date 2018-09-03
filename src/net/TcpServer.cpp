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
TcpServer::TcpServer(EventLoop* loop, SocketAddress server_address, Handler* handler, uint32_t sub_loop_num)
	: main_loop_(loop),
	sub_loop_num_(sub_loop_num),
	server_address_(server_address),
	acceptor_(new Acceptor(main_loop_, server_address_)),
	event_loop_thread_pool_(sub_loop_num),
	default_handler_(handler)
{
	acceptor_->set_reuse_address_option();
	acceptor_->set_init_connection_callback(std::bind(&TcpServer::init_connection, this, _1, _2));
	acceptor_->listen();
	LOG_INFO << "Server Started.";
}

TcpServer::TcpServer(EventLoop* loop, const char* ip, unsigned short port, Handler* handler, uint32_t sub_loop_num)
	: TcpServer(loop, SocketAddress(ip, port), handler, sub_loop_num)
{}

TcpServer::TcpServer(EventLoop* loop, unsigned short port, Handler* handler, uint32_t sub_loop_num)
	: TcpServer(loop, SocketAddress(port), handler, sub_loop_num)
{}

void TcpServer::on_new_connection(const TcpConnection_Ptr& connection_ptr) const
{
	if (new_connection_callback_) { new_connection_callback_(connection_ptr); }
}

void TcpServer::on_message_readable(const TcpConnection_Ptr& connection_ptr, Buffer* buffer)
{
	const SOCKET fd = connection_ptr->get_fd();
	if (!tcp_connection_map_[fd].second) { }
}


void TcpServer::on_message_writable(const TcpConnection_Ptr& connection_ptr, int count, Buffer* buffer) const
{
	if (message_writable_callback_) { message_writable_callback_(connection_ptr, count, buffer); }
}

void TcpServer::init_connection(int fd, const SocketAddress& client_address)
{
	//SocketSetNoblocking(fd); 暂时还不开启
	EventLoop* loop = event_loop_thread_pool_.get_next_event_loop();

	TcpConnection_Ptr new_connection_ptr = std::make_shared<TcpConnection>(loop, fd, client_address,
																			server_address_);
	LOG_TRACE << "Socket<" << fd << ">"
			  << " has been distributed to"
		      << " EventLoop<" << loop->get_event_loop_id() << ">.";

	new_connection_ptr->set_readable_callback(std::bind(&TcpServer::on_message_readable, this, _1, _2));
	new_connection_ptr->set_writable_callback(std::bind(&TcpServer::on_message_writable, this, _1, _2, _3));
	new_connection_ptr->set_closed_callback(std::bind(&TcpServer::deinit_connection, this, _1));
	new_connection_ptr->set_connection_style(TcpConnection::kEstablishing);
	tcp_connection_map_[fd].first = new_connection_ptr;


	tcp_connection_map_[fd].second.reset();

	loop->push_back_task(std::bind(&TcpServer::on_new_connection, this, new_connection_ptr));
}

void TcpServer::deinit_connection(const TcpConnection_Ptr& connection_ptr)
{
	connection_ptr->set_connection_style(connection_ptr->kDisconnected);

	tcp_connection_map_.erase(connection_ptr->get_fd());
}
}}
