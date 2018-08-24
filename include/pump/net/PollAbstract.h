//
// Created by fenng on 18-8-10.
//

#ifndef PUMP_BACKENDABSTRACT_H
#define PUMP_BACKENDABSTRACT_H


#include <vector>


#include <pump/Common.h>
#include <pump/net/EventLoop.h>

namespace pump { namespace net
{
//typedef std::vector<WatcherAbstract> HandleList;

class WatcherAbstract;

class PollAbstract
{
public:
	typedef std::function<void()> Task;
	typedef std::vector<Task> TaskList;
public:

	virtual ~PollAbstract() = 0
	{ }

	virtual void poll(const timeval* tv, TaskList* io_task_list) = 0;
	virtual void init_backend() = 0;

	virtual void add_interests(const WatcherAbstract& handle) = 0;
	virtual void modify_interests(const WatcherAbstract& handle) = 0;
	virtual void delete_interests(const WatcherAbstract& handle) = 0;
};
}}

#endif //PUMP_BACKENDABSTRACT_H
