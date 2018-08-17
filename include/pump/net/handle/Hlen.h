#ifndef PUMP_NET_SIMPLEMESSAGEPARSER_H_
#define  PUMP_NET_SIMPLEMESSAGEPARSER_H_

#include <cstdint>
#include <pump/net/TcpConnection.h>
#include <pump/BytesHelper.h>
#include <pump/net/TcpServer.h>

namespace pump { namespace net
{
template <typename HeaderType>
class SimpleMesageParser
{
public:
	typedef std::function<void(const TcpConnection_Ptr&, Buffer*)> MessageReadableCallback;
	typedef std::function<void(const TcpConnection_Ptr&, Buffer*)> MessageWritableCallback;
	typedef std::function<void(const TcpConnection_Ptr&, const std::string&)> CompletePackageCallback;

public:
	SimpleMesageParser(TcpServer* server)
		: server_(server)
	{
		server_->set_message_readable_callback(on_message);
	}


	static void on_message(const TcpConnection_Ptr& tcp_connection_ptr, Buffer* buffer)
	{
		HeaderType message_size = 0;
		HeaderType content_size = 0;
		if (buffer->get_readable_bytes() >= sizeof(HeaderType)) {
			content_size = pump::bigendian::Read_XX_Impl(buffer->get_readable_address(),
														pump::bigendian::Type<HeaderType>());
			message_size = content_size + sizeof(HeaderType);
		}

		if (buffer->get_readable_bytes() >= message_size) {
			const std::string content = decode_content(buffer->get_readable_address(), content_size);
			if (complete_package_callback_) {
				complete_package_callback_(tcp_connection_ptr, content);
			}
			buffer->retrieve(sizeof(HeaderType) + content_size);
		}
	}

	static void send(const TcpConnection_Ptr& tcp_connection_ptr, const std::string& content)
	{
		tcp_connection_ptr->send(encode_content(content));
	}

	static void set_complete_package_callback(const CompletePackageCallback& cb) { complete_package_callback_ = cb; }

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

private
:
	static CompletePackageCallback complete_package_callback_;
	TcpServer* server_;
};

template <typename HeaderType>
typename SimpleMesageParser<HeaderType>::CompletePackageCallback SimpleMesageParser<HeaderType>::
	complete_package_callback_ = nullptr;
}}


#endif
