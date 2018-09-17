#ifndef PUMP_NET_SIMPLEMESSAGEPARSER_H
#define PUMP_NET_SIMPLEMESSAGEPARSER_H

#include <pump/net/TcpConnection.h>
#include <pump/BytesHelper.h>
#include <pump/net/TcpServer.h>
#include <pump/Packet.h>
#include <pump/net/Handler.h>


namespace pump { namespace net
{

template <typename HeaderType>
class Hlen : public Handler
{
public:
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const TcpConnection_Ptr&, Buffer*) BufferReadableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const TcpConnection_Ptr&, Buffer*) BufferWritableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const TcpConnection_Ptr&, Packet*) CompletePacketCallback;

public:
	explicit Hlen(TcpServer* server, const TcpConnection_Ptr tcp_connection_ptr)
		: state_(parse_state::k_read_len),
		server_(server),
		tcp_connection_ptr_(tcp_connection_ptr)
	{
		server_->set_message_readable_callback(std::bind(&Hlen::on_message, this, std::placeholders::_1,
														std::placeholders::_2));
	}

	~Hlen() = default;


PUMP_DECLARE_DEFAULT_COPYABLE(Hlen)
PUMP_DECLARE_DEFAULT_MOVABLE(Hlen)

public:
	void on_message(const TcpConnection_Ptr& tcp_connection_ptr, Buffer* buffer) override
	{
		bool is_exit_loop = false;
		do {
			switch (state_) {
				case parse_state::k_read_len:
					if (buffer->get_readable_bytes() > sizeof(HeaderType)) {
						content_size_ = pump::bigendian::Read_XX_Impl(buffer->get_readable_address(),
																	pump::bigendian::Type<HeaderType>());
						const HeaderType message_size = content_size_ + sizeof(HeaderType);
						state_ = parse_state::k_read_content;
						buffer->retrieve(sizeof(HeaderType));
					}
					else {
						is_exit_loop = true;
					}
				case parse_state::k_read_content:
					if (buffer->get_readable_bytes() >= content_size_) {
						//const std::string content = decode_content(buffer->get_readable_address(), content_size_);
						tcp_connection_ptr->get_packet_address()->write(buffer->get_readable_address(), content_size_);

						if (complete_package_callback_) {
							complete_package_callback_(tcp_connection_ptr, tcp_connection_ptr->get_packet_address());
						}
						buffer->retrieve(content_size_);
						tcp

						state_ = parse_state::k_read_len;
					}
					else {
						is_exit_loop = true;
					}
			}
		} while (!is_exit_loop);
	}

	void send(const TcpConnection_Ptr& tcp_connection_ptr, const BYTE_T* content, size_t len) override
	{
		Packet send_packet;
		const auto head_size = static_cast<HeaderType>(len);
		send_packet.write_left(head_size);
		send_packet.write(content, len);

		tcp_connection_ptr->send_in_bind_thread(reinterpret_cast<const char*>(send_packet.begin()),
												send_packet.get_len());
	}

	void set_complete_package_callback(const CompletePacketCallback& cb) override { complete_package_callback_ = cb; }

	Handler* clone() override
	{
		return new Hlen(*this);
	}

private:
	enum parse_state
	{
		k_read_len,
		k_read_content,
	};


	parse_state state_;
	HeaderType content_size_;
private:

	CompletePacketCallback complete_package_callback_;
	TcpServer* server_;
	TcpConnection_Ptr tcp_connection_ptr_;
};



}}



#endif
