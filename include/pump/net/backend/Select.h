//
// Created by fenng on 18-8-13.
//

#ifndef PUMP_SELECT_H
#define PUMP_SELECT_H

#include <pump/net/BackendAbstract.h>

#include <functional>

namespace pump {namespace net
{
class Select : public BackendAbstract
{
public:
	explicit Select(EventLoop* loop)
		: loop_(loop), is_quit_(true),
		max_fd_(-1)
	{
	}

	void poll() override;

	void init_backend() override;

	void add_interests(const WatchAbstract& handle) override;
	void modify_interests(const WatchAbstract& handle) override;
	void delete_interests(const WatchAbstract& handle) override;

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
	typedef std::vector<std::reference_wrapper<const WatchAbstract>> EventList;

	ReadableList readable_list_;
	WritableList writable_list_;
	EventList event_list_;
};
}}
#endif //PUMP_SELECT_H
