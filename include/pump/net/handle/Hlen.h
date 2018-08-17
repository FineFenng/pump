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

	PUMP_DECLAE_CALLBACK_FUNCTIONR(void, const TcpConnection_Ptr&, Buffer*) BufferReadableCallback;
	PUMP_DECLAE_CALLBACK_FUNCTIONR(void, const TcpConnection_Ptr&, Buffer*) BufferWritableCallback;
	PUMP_DECLAE_CALLBACK_FUNCTIONR(void, const TcpConnection_Ptr&, Packet*) CompletePacketCallback;

public:
	explicit Hlen(TcpServer* server)
		: state_(parse_state::k_read_len), server_(server)
	{
		server_->set_message_readable_callback(on_message);
	}

PUMP_DECLARE_NONCOPYABLE(Hlen);

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
						const std::string content = decode_content(buffer->get_readable_address(), content_size_);
						package_.read(buffer->get_readable_address(), content_size_);

						if (complete_package_callback_) {
							complete_package_callback_(tcp_connection_ptr, &package_);
						}
						buffer->retrieve(content_size_);
					}
					else {
						is_exit_loop = true;
						break;
					}
					break;
			}
		} while (!is_exit_loop);
	}

	static void send(const TcpConnection_Ptr& tcp_connection_ptr, const std::string& content)
	{
		tcp_connection_ptr->send(encode_content(content));
	}

	static void set_complete_package_callback(const CompletePacketCallback& cb) { complete_package_callback_ = cb; }

private :
	static std::string encode_content(const std::string& content)
	{
		const int head_size = sizeof(HeaderType);
		std::string re;
		re.resize(head_size);
		pump::bigendian::Write_xx_impl(content.size(), re.c_str());
		re += content;
		return re;
	}


	static std::string decode_content(const char* message, size_t len)
	{
		return std::string(message + sizeof(HeaderType), len);
	}

private:
	enum parse_state
	{
		k_read_len,
		k_read_content,
	};

	parse_state state_;
	HeaderType content_size_;
	Packet package_;

private
:
	static CompletePacketCallback complete_package_callback_;
	TcpServer* server_;
};


template <typename HeaderType>
typename Hlen<HeaderType>::CompletePacketCallback Hlen<HeaderType>::
	complete_package_callback_ = nullptr;
}}


#endif
