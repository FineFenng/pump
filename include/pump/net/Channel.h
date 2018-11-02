//
// Created by fengc on 18-10-17.
//

#ifndef PUMP_NET_CHANNEL_H
#define PUMP_NET_CHANNEL_H

#include <pump/Common.h>
#include "pump/utility/log/LogLine.h"

namespace pump {namespace net {
	class EventLoop;
	class ChannelPipeline;
	class ChannelContext;

	class Channel : public pump::RefBase {
	PUMP_DECLARE_DELETE_COPYABLE_AND_MOVABLE(Channel)
	public:
		explicit Channel(EventLoop* loop)
			: loop_(loop),
			  channel_pipeline_(nullptr),
			  channel_context_(nullptr) {
			LOG_TRACE << "Channel::Channel()";
		}

		~Channel() {
			LOG_TRACE << "Channel::~Channel()";
		}


	private:
		PMRP<EventLoop> loop_;
		PMRP<ChannelPipeline> channel_pipeline_;
		PMRP<ChannelContext> channel_context_;
	};

}}

#endif //PUMP_NET_CHANNEL_H
