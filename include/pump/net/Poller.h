//
// Created by fenng on 18-8-10.
//


#ifndef PUMP_NET_POLLER_H_
#define PUMP_NET_POLLER_H_

#include <functional>
#include <vector>

#include <pump/Common.h>
#include <pump/net/Watcher.h>

namespace pump { namespace net
{
class Poller
{
public:
	typedef std::function<void()> Task;
	typedef std::vector<Task> TaskList;
public:
	virtual ~Poller() = default;

	virtual void poll(timeval* const tv, TaskList* io_task_list) = 0;
	virtual void init_backend() = 0;
	virtual void add_interests(const Watcher& handle) = 0;
	virtual void modify_interests(const Watcher& handle) = 0;
	virtual void delete_interests(const Watcher& handle) = 0;
};
}}

#endif
