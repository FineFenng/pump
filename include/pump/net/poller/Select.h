//
// Created by fenng on 18-8-13.
//

#ifndef PUMP_SELECT_H
#define PUMP_SELECT_H

#include <ctime>
#include <cassert>
#include <functional>

#include <pump/Common.h>
#include <pump/net/PollerInterface.h>

namespace pump {namespace net
{
class EventLoop;

class Select : public PollerInterface
{
public:
	explicit Select(EventLoop* loop)
		: loop_(loop),
		is_quit_(true),
		max_fd_(0)
	{ }

	~Select() override
	{
		assert(watcher_list_.empty());
	}

PUMP_DECLARE_DELETE_COPYABLE(Select)
PUMP_DECLARE_DELETE_MOVABLE(Select)

	void poll(const timeval* tv, TaskList* io_task_list) override;

	void init() override;
	void destroy() override;

	void add_interests(const WatcherInterface& watcher) override;
	void modify_interests(const WatcherInterface& watcher) override;
	void delete_interests(const WatcherInterface& watcher) override;

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
	typedef std::vector<std::reference_wrapper<const WatcherInterface>> WatcherList;

	ReadableList readable_list_;
	WritableList writable_list_;
	WatcherList watcher_list_;
};
}}
#endif //PUMP_SELECT_H
