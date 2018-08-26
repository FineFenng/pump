#ifndef PUMP_NET_PLATFORM_H_
#define PUMP_NET_PLATFORM_H_

#include <pump/Common.h>

#ifdef PUMP_PLATFORM_GNU
#include <pump/net/poll/Select.h>
#include <pump/net/poll/Poll.h>
#endif

#ifdef PUMP_PLATFORM_LINUX
#include <pump/net/poll/Select.h>
#include <pump/net/poll/Poll.h>
#include <pump/net/poll/Epoll.h>
#endif

#ifdef PUMP_PLATFORM_MACX
#include <pump/net/poll/Select.h>
#include <pump/net/poll/Poll.h>
#include <pump/net/poll/Kqueue.h>
#endif

#ifdef PUMP_PLATFORM_WIN
#include <pump/net/poll/Select.h>
#endif

namespace pump {namespace net
{

    enum class PollType
    {
        kDefault,
        kSelect,
#ifdef PUMP_PLATFORM_MACX
        kKQueue,
#endif
#ifdef PUMP_PLATFORM_LINUX
        kEpoll,
#endif
    };

enum EventFlag : unsigned int {
    kUndefined  = 0xFFFFFFFF,
    kNone       = 0x00,
    kIOReadable = 0x01,
    kIOWritable = 0x02,
    kIO         = kIOReadable | kIOWritable,
    kTimerout   = 0x04,
};

}}

#endif
