#ifndef PUMP_NET_HANDLEINTERFACE_H
#define PUMP_NET_HANDLEINTERFACE_H
#include <pump/Common.h>

namespace pump { namespace net {
	class ChannelHandlerContext;
	class Packet;

	class ChannelHandlerInterface {
	public:
		virtual ~ChannelHandlerInterface() = default;
	};

	class ActivityHandlerInterface : virtual public ChannelHandlerInterface {
	public:
		virtual void connected(const ChannelHandlerContext& context);
		virtual void closed(const ChannelHandlerContext& context) = 0;
		virtual void readable_shutdown(const ChannelHandlerContext& context) = 0;
		virtual void writable_shutdown(const ChannelHandlerContext& context) = 0;
		virtual void write_block(const ChannelHandlerContext& context) = 0;
		virtual void write_unblocked(const ChannelHandlerContext& context) = 0;
	};

	class InboundHandlerInterface : virtual public ChannelHandlerInterface {
	public:
		virtual void read(const ChannelHandlerContext& context, const Packet& income) = 0;
	};

	class OutboundHandlerInterface : virtual public ChannelHandlerInterface {
	public:
		virtual void write(const ChannelHandlerContext& context, const Packet& outlet) = 0;
		virtual void flush(const ChannelHandlerContext& context) = 0;
		virtual void close(const ChannelHandlerContext& context) = 0;
		virtual void shutdown_read(const ChannelHandlerContext& context) = 0;
		virtual void shutdown_write(const ChannelHandlerContext& context) = 0;
	};

	class HandlerHead : public OutboundHandlerInterface {
	public:
		void write(const ChannelHandlerContext& context, const Packet& outlet) override;
		void flush(const ChannelHandlerContext& context) override;
		void close(const ChannelHandlerContext& context) override;
		void shutdown_read(const ChannelHandlerContext& context) override;
		void shutdown_write(const ChannelHandlerContext& context) override;

	};

	class HandlerTail : public ActivityHandlerInterface,
	                    public InboundHandlerInterface {
	public:
		void connected(const ChannelHandlerContext& context) override;
		void closed(const ChannelHandlerContext& context) override;
		void readable_shutdown(const ChannelHandlerContext& context) override;
		void writable_shutdown(const ChannelHandlerContext& context) override;
		void write_block(const ChannelHandlerContext& context) override;
		void write_unblocked(const ChannelHandlerContext& context) override;
		void read(const ChannelHandlerContext& context, const Packet& income) override;
	};


}}
#endif
