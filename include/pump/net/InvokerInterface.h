#ifndef PUMP_NET_INVOKERINTERFACE_H
#define PUMP_NET_INOVKERINTERFACE_H

namespace pump { namespace net {
	class Packet;

	class ActivityInvokerInterface {
	public:
		virtual ~ActivityInvokerInterface() = default;
		virtual void fire_connected() = 0;
		virtual void fire_closed() = 0;
		virtual void fire_readable_shutdown() = 0;
		virtual void fire_writable_shutdown() = 0;
		virtual void fire_write_blocked() = 0;
		virtual void fire_write_unblocked() = 0;
	};

	class InboundInvokerInterface {
	public:
		virtual ~InboundInvokerInterface() = default;
		virtual void fire_read(const Packet& income) = 0;
	};

	class OutboundInvokerInterface {
	public:
		virtual ~OutboundInvokerInterface() = default;
		virtual void write(const Packet& income) = 0;
		virtual void close(const Packet& income) = 0;
		virtual void shutdown_read() = 0;
		virtual void shutdown_write() = 0;
		virtual void flush() = 0;
	};



}}
#endif
