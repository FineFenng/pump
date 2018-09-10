#ifndef PUMP_NET_HANDLER_H_
#define PUMP_NET_HANDLER_H_

#include <pump/net/TcpConnection.h>

namespace pump {
}
namespace pump { namespace net
{

class Handler
{
public:
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const TcpConnection_Ptr&, Buffer*) BufferReadableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const TcpConnection_Ptr&, Buffer*) BufferWritableCallback;
	PUMP_DECLARE_CALLBACK_FUNCTION(void, const TcpConnection_Ptr&, Packet*) CompletePacketCallback;

public:
	virtual ~Handler() {}

public:
	virtual void on_message(const TcpConnection_Ptr& tcp_connection_ptr, Buffer* buffer) = 0;
	virtual void send(const TcpConnection_Ptr& tcp_connection_ptr, const BYTE_T* content, size_t len);
	virtual void set_complete_package_callback(const CompletePacketCallback& cb) = 0;
	virtual Handler* clone() = 0;
};
}}
#endif
