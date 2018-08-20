//
// Created by finefenng on 2018/7/5.
//

#include <pump/net/EventLoop.h>

#include <pump/net/Acceptor.h>
#include <pump/net/BackendAbstract.h>
#include <pump/Common.h>
#include <pump/net/WatchAbstract.h>
#include <pump/net/backend/Platform.h>

/*
 * struct kevent {
 *     uintptr_t ident;        事件 ID
 *     short     filter;       事件过滤器
 *     u_short   flags;        行为标识
 *     u_int     fflags;       过滤器标识值
 *     intptr_t  data;         过滤器数据
 *     void      *udata;       应用透传数据
 * };
 */

/*
 * struct epoll_event {
 *      uint32_t     events;
 *      epoll_data_t data;
 * }
 *
 * typedef union epoll_data {
 *      void*     ptr;
 *      int        fd;
 *      uint32_t  u32;
 *      uint64_t  u64;
 *  } epoll_data_t;
 *
 */

namespace pump {namespace net
{
thread_local EventLoop* t_eventLoop = nullptr;

EventLoop::EventLoop()
	: is_looping_(false),
	backend_(),
	threadId_(std::this_thread::get_id())
{
	if (t_eventLoop == nullptr) {
		t_eventLoop = this;
	}

#if PUMP_PLATFORM == PUMP_PLATFORM_GNU
        backend_.reset(new Select(this));
#elif PUMP_PLATFORM == PUMP_PLATFORM_LINUX
	BackendType g_default_backend_type = BackendType::kEpoll;
        backend_.reset(new Epoll(this));
#elif  PUMP_PLATFORM == PUMP_PLATFORM_MACX
        backend_.reset(new KQueue(this));
#elif  PUMP_PLATFORM == PUMP_PLATFORM_WIN
	backend_.reset(new Select(this));
#else
	#error
#endif

	backend_->init_backend();
}

EventLoop::~EventLoop()
{
	LOG_INFO << "Event loop terminate.";
}

void EventLoop::run()
{
	is_looping_ = true;

	while (is_looping_) {
		backend_->poll();
	}
	assert(!is_looping_);
	is_looping_ = false;
}

void EventLoop::update_handle(const WatchAbstract& handle) const
{
	if (handle.get_index() < 0) {
		add_handle(handle);
	}
	else {
		modify_handle(handle);
	}
}

void EventLoop::remove_handle(const WatchAbstract& handle) const
{
	delete_handle(handle);
}

void EventLoop::add_handle(const WatchAbstract& handle) const
{
	backend_->add_interests(handle);
}

void EventLoop::modify_handle(const WatchAbstract& handle) const
{
	backend_->modify_interests(handle);
}

void EventLoop::delete_handle(const WatchAbstract& handle) const
{
	backend_->delete_interests(handle);
}

bool EventLoop::isInInitThread()
{
	return threadId_ == std::this_thread::get_id();
}
}}
