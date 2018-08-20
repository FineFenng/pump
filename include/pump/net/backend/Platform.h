#ifndef PUMP_NET_PLATFORM_H_
#define PUMP_NET_PLATFORM_H_

#include <pump/Common.h>

#ifdef PUMP_PLATFORM_GNU
#include <pump/net/backend/Select.h>
#include <pump/net/backend/Poll.h>
#endif

#ifdef PUMP_PLATFORM_LINUX
#include <pump/net/backend/Select.h>
#include <pump/net/backend/Poll.h>
#include <pump/net/backend/Epoll.h>
#endif

#ifdef PUMP_PLATFORM_MACX
#include <pump/net/backend/Select.h>
#include <pump/net/backend/Poll.h>
#include <pump/net/backend/Kqueue.h>
#endif

#ifdef PUMP_PLATFORM_WIN
#include <pump/net/backend/Select.h>
#endif

namespace pump {namespace net
{

    enum class BackendType
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
}}

#endif
