#ifndef PUMP_NET_SIMPLEMESSAGEPARSER_H_
#define  PUMP_NET_SIMPLEMESSAGEPARSER_H_

#include <pump/net/TcpConnection.h>
#include <pump/BytesHelper.h>
#include <pump/net/TcpServer.h>
#include <pump/Packet.h>


namespace pump { namespace net
{
template <typename HeaderType>
class Hlen
{
public:
	//typedef std::function<void(const TcpConnection_Ptr&, Buffer*)> BufferReadableCallback;
	//typedef std::function<void(const TcpConnection_Ptr&, Buffer*)> BufferWritableCallback;
	//typedef std::function<void(const TcpConnection_Ptr&, const std::string&)> CompletePacketCallback;

	PUMP_DECLARE_CALLBACK_FUNCTION(void, const TcpConnection_Ptr&, Buffer*) BufferReadableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const TcpConnection_Ptr&, Buffer*) BufferWritableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const TcpConnection_Ptr&, Packet*) CompletePacketCallback;

public:
	explicit Hlen(TcpServer* server)
		: state_(parse_state::k_read_len)
		, server_(server)
	{
		server_->set_message_readable_callback(std::bind(&Hlen::on_message, this, std::placeholders::_1,
														std::placeholders::_2));
	}

PUMP_DECLARE_NON_COPYABLE(Hlen)

public:
	void on_message(const TcpConnection_Ptr& tcp_connection_ptr, Buffer* buffer)
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
						break;
					}
				case parse_state::k_read_content:
					if (buffer->get_readable_bytes() >= content_size_) {
						//const std::string content = decode_content(buffer->get_readable_address(), content_size_);
						recv_packet_.write(buffer->get_readable_address(), content_size_);

						if (complete_package_callback_) {
							complete_package_callback_(tcp_connection_ptr, &recv_packet_);
						}
						buffer->retrieve(content_size_);
						state_ = parse_state::k_read_len;
						recv_packet_.reset();
					}
					else {
						is_exit_loop = true;
						break;
					}
					break;
			}
		} while (!is_exit_loop);
	}

	void send(const TcpConnection_Ptr& tcp_connection_ptr, const BYTE_T* content, size_t len)
	{
		send_packet_.reset();
		encode_content(content, len);
		tcp_connection_ptr->send_in_bind_thread(reinterpret_cast<const char*>(send_packet_.begin()), send_packet_.get_len());
	}

	void set_complete_package_callback(const CompletePacketCallback& cb) { complete_package_callback_ = cb; }

private :
	void encode_content(const BYTE_T* content, size_t len)
	{
		const auto head_size = static_cast<HeaderType>(len);
		send_packet_.write_left(head_size);
		send_packet_.write(content, len);
	}


	void decode_content(const char* message, size_t len)
	{ }

private:
	enum parse_state
	{
		k_read_len,
		k_read_content,
	};

	parse_state state_;
	HeaderType content_size_;
	Packet recv_packet_;
	Packet send_packet_;
private
:
	CompletePacketCallback complete_package_callback_;
	TcpServer* server_;
};
}}


#endif
