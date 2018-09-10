//
// Created by fenng on 18-8-13.
//

#ifndef PUMP_SELECT_H
#define PUMP_SELECT_H

#include <pump/net/Poller.h>

#include <functional>
#include <cassert>

namespace pump {namespace net
{
class EventLoop;

class Select : public PollAbstract
{
public:
	explicit Select(EventLoop* loop)
		: loop_(loop), is_quit_(true),
		max_fd_(-1)
	{ }

	~Select() override
	{
		assert(watcher_list_.empty());
	}


	void poll(timeval* const tv, TaskList* io_task_list) override;

	void init_backend() override;

	void add_interests(const watcher& watcher) override;
	void modify_interests(const watcher& watcher) override;
	void delete_interests(const watcher& watcher) override;

private:
	

	enum Select_Flag
	{
		kZeroFd,
		kSetFd,
		kCtlFd,
	};


private:
	EventLoop* loop_;
	bool is_quit_;
	SOCKET max_fd_;

private:
	typedef fd_set ReadableList;
	typedef fd_set WritableList;
	typedef std::vector<std::reference_wrapper<const watcher>> WatcherList;

	ReadableList readable_list_;
	WritableList writable_list_;
	WatcherList watcher_list_;
};
}}
#endif //PUMP_SELECT_H
