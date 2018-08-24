//
// Created by finefenng on 2018/7/5.
//

#ifndef PUMP_NET_EVENTLOOP_H_
#define PUMP_NET_EVENTLOOP_H_

#include <map>
#include <memory>
#include <mutex>
#include <thread>

#include <pump/Common.h>
#include <pump/net/BackendAbstract.h>
#include <pump/net/backend/Platform.h>
#include <pump/net/SocketOption.h>

namespace pump {namespace net
{
class BackendAbstract;
class WatcherAbstract;
class IO_Watcher;

class EventLoop
{
public:
	typedef std::function<void()> Task;
public:
	EventLoop();

	~EventLoop();

PUMP_DECLARE_NONCOPYABLE(EventLoop)
PUMP_DECLARE_NONMOVABLE(EventLoop)

	void run();

	void update_watcher(const WatcherAbstract& watcher) const;

	void remove_watcher(const WatcherAbstract& watcher) const;

	void push_back_task(const Task& task)
	{
		if (is_in_init_thread()) {
			normal_task_list_.push_back(task);
		}
		else {
			{
				std::lock_guard<std::mutex> lock_guard(mutex_);
				normal_task_list_.push_back(task);
			}
			wakeup();
		}
	}

private:
	void add_watcher(const WatcherAbstract& watcher) const;

	void modify_watcher(const WatcherAbstract& watcher) const;

	void delete_watcher(const WatcherAbstract& watcher) const;

	void wakeup() const;

private:
	void init();

	bool is_in_init_thread() const;

	int calculate_block_time() const;

	void init_backend();
	void init_notify_watcher();
	void clear_wakup_fd_buffer() const;


private:
	enum LoopState
	{
		kNormaTasklExecute,
		kIOPolling,
		kIOTaskExecute,
		kDelayTaskExecute,
	};

private:
	uint64_t thread_id_;
	bool is_looping_;
	LoopState loop_state_;

private:
	SOCKET wakeup_w_fd_;
	SOCKET wakeup_r_fd_;
	std::unique_ptr<IO_Watcher> wakeup_watcher_;

private:
	std::unique_ptr<BackendAbstract> backend_;
	BackendType backend_type_;
private:
	typedef std::function<void()> Task;
	typedef std::vector<Task> TaskList;

	TaskList normal_task_list_;
	TaskList delay_task_list_;
	TaskList io_task_list_;
	std::mutex mutex_;
};
}}

#endif //QPSTEST_EVENTLOOP_H
