//
// Created by finefenng on 2018/7/5.
//

#include <pump/net/EventLoop.h>

#include <sstream>

#include <pump/net/Acceptor.h>

#include <pump/utility/thread/ThreadOption.h>

/*
 * struct kevent {
 *     uintptr_t ident;        
 *     short     filter;       
 *     u_short   flags;        
 *     u_int     fflags;       
 *     intptr_t  data;         
 *     void      *udata;       
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

namespace pump { namespace net {
namespace {
/*
uint64_t GetCurrentThreadId()
{
	std::ostringstream out_stream;
	out_stream << std::this_thread::get_id();
	const uint64_t current_thread_id = std::strtoll(out_stream.str().c_str(), nullptr, 16);

	return current_thread_id;
}
*/

thread_local EventLoop* t_event_loop = nullptr;
thread_local uint64_t t_thread_id = pump::utility::GetCurrentThreadId();
}

EventLoop::EventLoop()
	: thread_id_(t_thread_id),
	  is_looping_(false),
	  loop_state_(LoopState::kNormaTasklExecute),
	  w_wakeup_fd_(-1),
	  r_wakeup_fd_(-1),
	  poll_type_(PollType::kDefault)
{
	if (t_event_loop == nullptr) {
		t_event_loop = this;
	}
	else {
		LOG_FALAL << "There exites an eventLoop Object in thread " << thread_id_;;
		//abort();
	}
	LOG_INFO << "EventLoop: " << this << " has been built in thread " << thread_id_;

	init();
}

EventLoop::~EventLoop()
{
	assert(!is_looping_);
	SocketClose(w_wakeup_fd_);
	SocketClose(r_wakeup_fd_);

	//TODO if there exits tasks in normal, delay or io task list. 

	LOG_INFO << "Event loop terminate.";
	init();
}

void EventLoop::run()
{
	assert(!is_looping_);
	is_looping_ = true;
	do {
		switch (loop_state_) {
			case kNormaTasklExecute: {
				TaskList backup_task_list;
				{
					std::lock_guard<std::mutex> guard(mutex_);
					if (!normal_task_list_.empty()) {
						std::swap(normal_task_list_, backup_task_list);
					}
				}
				for (auto& i : backup_task_list) {
					i();
				}
				loop_state_ = LoopState::kIOPolling;
			}
			case kIOPolling: {
				const int      wait_time = calc_poll_block_time();
				struct timeval temp_tv   = {0, 0};
				struct timeval* tv = nullptr;

				if (wait_time == 0) {
					temp_tv.tv_sec  = 0L;
					temp_tv.tv_usec = 0L;
					tv = &temp_tv;
				}

				if (wait_time > 0) {


				}

				/*
				if (wait_time < 0) {
				    temp_tv = nullptr;
				*/

				clear_wakup_fd_buffer();
				poll_->poll(tv, &io_task_list_);
				loop_state_ = LoopState::kIOTaskExecute;
			}
			case kIOTaskExecute: {
				for (auto& i : io_task_list_) {
					i();
				}
				loop_state_ = LoopState::kDelayTaskExecute;
			}
			case kDelayTaskExecute: {
				for (auto& i : delay_task_list_) {
					i();
				}
				loop_state_ = LoopState::kNormaTasklExecute;
			}
		}
	} while (is_looping_);
}

void EventLoop::update_watcher(const WatcherAbstract& watcher) const
{
	if (watcher.get_index() < 0) {
		add_watcher(watcher);
	}
	else {
		modify_watcher(watcher);
	}
}

void EventLoop::remove_watcher(const WatcherAbstract& watcher) const
{
	delete_watcher(watcher);
}

void EventLoop::add_watcher(const WatcherAbstract& watcher) const
{
	poll_->add_interests(watcher);
}

void EventLoop::modify_watcher(const WatcherAbstract& watcher) const
{
	poll_->modify_interests(watcher);
}

void EventLoop::delete_watcher(const WatcherAbstract& watcher) const
{
	poll_->delete_interests(watcher);
}

void EventLoop::wakeup() const
{
	char val[5] = {0};
	//TODO
	int  saved_errno;
	Send(w_wakeup_fd_, val, sizeof(val), 0, &saved_errno);
}

void EventLoop::init()
{
	init_backend();
	init_notify_watcher();
}

bool EventLoop::is_in_bind_thread() const
{
	return thread_id_ == t_thread_id;
}

int EventLoop::calc_poll_block_time() const
{
	if (!delay_task_list_.empty()) {
		//TODO
		return 0;
	}

	if (delay_task_list_.empty() && normal_task_list_.empty()) {
		return -1;
	}

	if (delay_task_list_.empty() && !normal_task_list_.empty()) {
		return 0;
	}
	return -1;
}

void EventLoop::init_backend()
{
#if PUMP_PLATFORM == PUMP_PLATFORM_GNU
	poll_.reset(new Select(this));
	poll_type_ = PollType::kSelect;
#elif PUMP_PLATFORM == PUMP_PLATFORM_LINUX
	poll_.reset(new Epoll(this));
	poll_type_ = PollType::kEpoll;
#elif  PUMP_PLATFORM == PUMP_PLATFORM_MACX
	poll_.reset(new KQueue(this));
	poll_type_ = PollType::kKQueue;
#elif  PUMP_PLATFORM == PUMP_PLATFORM_WIN
	poll_.reset(new Select(this));
	poll_type_ = PollType::kSelect;
#else
	#error
#endif
	poll_->init_backend();
}

void EventLoop::init_notify_watcher()
{
	SOCKET sv[2];
	//TODO
	SocketGetInetStreamPair(SOCK_STREAM, sv);
	w_wakeup_fd_ = sv[0];
	r_wakeup_fd_ = sv[1];
	SocketSetNoblocking(r_wakeup_fd_);
	wakeup_watcher_.reset(new IO_Watcher(this, r_wakeup_fd_));
	//wakeup_watcher_->set_readable_callback([](){});
	wakeup_watcher_->enable_readable();
}

void EventLoop::clear_wakup_fd_buffer() const
{
	//TODO
	char recv_buffer[1024] = {0};
	::recv(r_wakeup_fd_, recv_buffer, sizeof(recv_buffer), 0);
}
}}
