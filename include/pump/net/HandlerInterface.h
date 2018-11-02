#ifndef PUMP_NET_HANDLEINTERFACE_H
#define PUMP_NET_HANDLEINTERFACE_H
#include <pump/Common.h>

namespace pump { namespace net {
	class ChannelContext;
	class Packet;

	class HandlerInterface {
	public:
		virtual ~HandlerInterface() = default;
	};

	class ActivityHandlerInterface : virtual public HandlerInterface {
	public:
		virtual void connected(const ChannelContext& context);
		virtual void closed(const ChannelContext& context) = 0;
		virtual void readable_shutdown(const ChannelContext& context) = 0;
		virtual void writable_shutdown(const ChannelContext& context) = 0;
		virtual void write_block(const ChannelContext& context) = 0;
		virtual void write_unblocked(const ChannelContext& context) = 0;
	};

	class InboundHandlerInterface : virtual public HandlerInterface {
	public:
		virtual void read(const ChannelContext& context, const Packet& income) = 0;
	};

	class OutboundHandlerInterface : virtual public HandlerInterface {
	public:
		virtual void write(const ChannelContext& context, const Packet& outlet) = 0;
		virtual void flush(const ChannelContext& context) = 0;
		virtual void close(const ChannelContext& context) = 0;
		virtual void shutdown_read(const ChannelContext& context) = 0;
		virtual void shutdown_write(const ChannelContext& context) = 0;
	};

	class HandlerHead : public OutboundHandlerInterface {
	public:
		void write(const ChannelContext& context, const Packet& outlet) override;
		void flush(const ChannelContext& context) override;
		void close(const ChannelContext& context) override;
		void shutdown_read(const ChannelContext& context) override;
		void shutdown_write(const ChannelContext& context) override;

	};

	class HandlerTail : public ActivityHandlerInterface,
	                    public InboundHandlerInterface {
	public:
		void connected(const ChannelContext& context) override;
		void closed(const ChannelContext& context) override;
		void readable_shutdown(const ChannelContext& context) override;
		void writable_shutdown(const ChannelContext& context) override;
		void write_block(const ChannelContext& context) override;
		void write_unblocked(const ChannelContext& context) override;
		void read(const ChannelContext& context, const Packet& income) override;
	};


}}
#endif
