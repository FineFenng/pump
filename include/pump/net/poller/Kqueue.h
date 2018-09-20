//
// Created by finefenng on 2018/8/26.
//


#ifndef PUMP_KQUEUE_H
#define PUMP_KQUEUE_H

#include <pump/net/Poller.h>

namespace pump {
namespace net {

class Kqueue : public Poller {
public:
	Kqueue();

private:
	void poll(const timeval* tv, TaskList* io_task_list) override;
	void init_backend() = 0;
	void add_interests(const Watcher& handle) override;
	void modify_interests(const Watcher& handle) override;
	void delete_interests(const Watcher& handle) override;
};

}

}

#endif //PUMP_KQUEUE_H
