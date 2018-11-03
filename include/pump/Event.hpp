#ifndef PUMP_EVENT_H
#define PUMP_EVENT_H

#include <map>
#include <type_traits>
#include <vector>

#include <pump/Common.h>

namespace pump {

static std::atomic<long> s_event_handler_id(0);

class EventHandlerBase : public pump::RefBase {
public:
	EventHandlerBase()
		: id_(s_event_handler_id.fetch_add(1)) {
	}

	long get_id() const {
		return id_;
	}

private:
	long id_;

};

template <typename Callable>
class EventHandler {
public:
	EventHandler(Callable&& callee):
		callee_(std::forward<Callable>(callee)) {
	}

	template <typename ...Args>
	void call(Args&& ...args) {
		callee_(std::forward<Callable>(args)...);
	}

private:
	typename std::remove_reference<Callable>::type callee_;

};

template <typename Callable>
PMRP<EventHandler<Callable>> MakeEventHandler(Callable&& callee) {
	return pump::MakeRef<EventHandler<Callable>>(std::forward<Callable>(callee));
}

class EventTrigger {
public:
	typedef std::vector<PMRP<EventHandlerBase>> HandlerVector;
	typedef std::map<int, HandlerVector> EventMap;
public:
	EventTrigger() = default;
	virtual ~EventTrigger() = default;

	void insert_into_map(const int id, const PMRP<EventHandlerBase>& handler) {
		auto it = handlers_.find(id);
		if (it != handlers_.end()) {
			it->second.push_back(handler);
			return;
		}
		HandlerVector handlers;
		handlers.push_back(handler);
		handlers_.insert({id, handlers});
	}

	void unbind(const long handler_id) {
		auto it = handlers_.begin();
		while (it != handlers_.end()) {
			const auto it_handlers = it->second.begin();
			while (it_handlers != it->second.end()) {
				if (handler_id == (*it_handlers)->get_id()) {
					it->second.erase(it_handlers);
				}
				return;
			}
			++it;
		}
	}


private:
	EventMap handlers_;
};


}
#endif
