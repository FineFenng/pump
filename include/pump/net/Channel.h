//
// Created by fengc on 18-10-17.
//

#ifndef PUMP_NET_CHANNEL_H
#define PUMP_NET_CHANNEL_H

namespace pump {
namespace net {
class EventLoop;
class ChannelPipeline;
class WathcherInterface;
class Channel {
 public:
  Channel(EventLoop* loop, WathcherInterface* watcher, ChannelPipeline* channel_pipeline);





 private:
  EventLoop* loop_;
  WathcherInterface* wathcher_;
  ChannelPipeline* channel_pipeline_;
};

}
}

#endif //PUMP_NET_CHANNEL_H
