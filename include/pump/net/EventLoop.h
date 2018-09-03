//
// Created by finefenng on 2018/7/5.
//

#ifndef PUMP_NET_EVENTLOOP_H_
#define PUMP_NET_EVENTLOOP_H_

#include <memory>
#include <mutex>
#include <thread>

#include <pump/Common.h>
#include <pump/net/PollAbstract.h>
#include <pump/net/Platform.h>

namespace pump {namespace net
{
class PollAbstract;
class WatcherAbstract;
class IO_Watcher;

class EventLoop
{
public:
	PUMP_DECLARE_CALLBACK_FUNCTION(void) Task;

public:
	explicit EventLoop(int event_loop_id = 0);

	~EventLoop();

PUMP_DECLARE_NON_COPYABLE(EventLoop)
PUMP_DECLARE_NON_MOVABLE(EventLoop)

	void run();

	void update_watcher(const WatcherAbstract& watcher);

	void remove_watcher(const WatcherAbstract& watcher);

	void remove_watcher_sync(const WatcherAbstract& watcher) const;

	void push_back_task(const Task& task)
	{
		if (is_in_bind_thread()) {
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

	void add_watcher(const WatcherAbstract& watcher) const;

	void modify_watcher(const WatcherAbstract& watcher) const;

	void delete_watcher(const WatcherAbstract& watcher) const;

	void wakeup() const;

	int get_event_loop_id() const { return event_loop_id_; }

private:
	void init();

	bool is_in_bind_thread() const;

	int calc_poll_block_time() const;

	void init_notify_watcher();

	void init_poll();

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
	SOCKET w_wakeup_fd_;
	SOCKET r_wakeup_fd_;
	std::unique_ptr<IO_Watcher> wakeup_watcher_;
	std::unique_ptr<PollAbstract> poll_;
	PollType poll_type_;

private:
	typedef std::vector<Task> TaskList;

	TaskList normal_task_list_;
	TaskList delay_task_list_;
	TaskList io_task_list_;
	std::mutex mutex_;
	int event_loop_id_;
};
}}

#endif //QPSTEST_EVENTLOOP_H
