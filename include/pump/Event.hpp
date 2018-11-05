#ifndef PUMP_EVENT_H
#define PUMP_EVENT_H

#include <map>
#include <functional>
#include <type_traits>
#include <vector>

#include <pump/Common.h>

namespace pump {

struct HandlerId {
public:
	HandlerId(long id)
		: id_(id) {
	}

	long id_;
};

inline bool operator==(const HandlerId& lhs, const HandlerId& rhs) {
	return lhs.id_ == rhs.id_;
}

inline bool operator!=(const HandlerId& lhs, const HandlerId& rhs) {
	return !(lhs == rhs);
}


static std::atomic<long> s_event_handler_id(0);

class EventHandlerBase : public pump::RefBase {
public:
	EventHandlerBase()
		: handler_id_(s_event_handler_id.fetch_add(1)) {
	}

	long get_handler_id() const {
		return handler_id_.id_;
	}

private:

	struct HandlerId handler_id_;

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
PUMP_DECLARE_DELETE_COPYABLE_AND_MOVABLE(EventTrigger)
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

	void unbind(const HandlerId& handler_id) {
		auto it = handlers_.begin();
		while (it != handlers_.end()) {
			const auto it_handlers = it->second.begin();
			while (it_handlers != it->second.end()) {
				if (handler_id == (*it_handlers)->get_handler_id()) {
					it->second.erase(it_handlers);
				}
				return;
			}
			++it;
		}
	}

	template <typename Callable>
	HandlerId bind(int event_id, Callable&& event_callee) {
		static_assert(std::is_class<std::remove_reference<Callable>>::value, "Callable must be callable object");
		const PMRP<EventHandlerBase> EventHandler = MakeEventHandler<Callable>(std::forward<Callable>(event_callee));
		insert_into_map(event_id, EventHandler);
		return EventHandler->get_handler_id();
	}

	template <typename CallableHint, typename Callable,
			class = typename std::enable_if<std::is_convertible<Callable, CallableHint>::value>::type>
	HandlerId bind(int event_id, Callable&& event_callee) {
		static_assert(std::is_class<std::remove_reference<Callable>>::value, "Callable must be callable object");
		const PMRP<EventHandlerBase> EventHandler = MakeEventHandler<Callable>(std::forward<Callable>(event_callee));
		insert_into_map(event_id, EventHandler);
		return EventHandler->get_handler_id();
	}

	template <typename CallableHint, typename Function, typename ...Args>
	HandlerId bind(int event_id, Function&& func, Args&&... args) {
		const PMRP<EventHandlerBase> EventHandler =
			MakeEventHandler<CallableHint>(std::bind(std::forward<Function>(func), std::forward<Args>(args)...));
		insert_into_map(event_id, EventHandler);
		return EventHandler->get_handler_id();
	}
	template<typename CallableHint, typename ...Args>
	void invoke(int event_id, Args&&... args) {
		auto it = handlers_.find(event_id);
		if(it!= handlers_.end()) {
			auto it_handler = it->second.begin();
			while (it_handler != it->second.end()) {
				PMRP<EventHandler<CallableHint>> callee_handler = *(it_handler++);
				callee_handler->call(std::forward<Args>(args)...);
			}
		}
	}

private:
	EventMap handlers_;
};

}
#endif
