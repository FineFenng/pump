//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_EVENTLOOP_H
#define QPSTEST_EVENTLOOP_H

#include <memory>
#include <thread>

#include <pump/Common.h>
#include <pump/net/BackendAbstract.h>
#include <pump/net/backend/Platform.h>

namespace pump {namespace net
{
class BackendAbstract;
class WatchAbstract;

class EventLoop
{
public:
    EventLoop();

	~EventLoop();

PUMP_DECLARE_NONCOPYABLE(EventLoop);

	void run();

	void update_handle(const WatchAbstract& handle) const;

	void remove_handle(const WatchAbstract& handle) const;

private:
	void add_handle(const WatchAbstract& handle) const;

	void modify_handle(const WatchAbstract& handle) const;

	void delete_handle(const WatchAbstract& handle) const;

private:
	bool isInInitThread();


private:
	bool is_looping_;
	std::unique_ptr<BackendAbstract> backend_;

private:
	std::thread::id threadId_;
};
}}

#endif //QPSTEST_EVENTLOOP_H
