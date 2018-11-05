#ifndef PUMP_FUTURE_H
#define PUMP_FUTURE_H

#include <condition_variable>
#include <mutex>

#include <pump/Event.hpp>
#include <pump/Common.h>

namespace pump {

template <typename T>
class Future : public pump::RefBase,
				protected pump::EventTrigger {
public:
	enum FutureState {
		kIdle,
		kCancelled,
		kSuccess,
		kFailure,
	};

	enum FutureEventStatus {
		kComplete,
	};

	typedef std::function<void(const PMRP<Future<T>>& future)> OperationCompleteFunction;
	typedef std::vector<FutureState> ListenerHandlerVector;

protected:
	void notify_waiter() {
		cond_.notify_all();
	}

	void notify_listeners() {
		if (listener_handlers_.empty()) {
			return;
		}

		EventTrigger::invoke<std::function<void()>>(kComplete);
		for (auto it : listener_handlers_) {
			EventTrigger::unbind(it);
		}
		listener_handlers_.clear();
	}

	template <typename Callable,
			typename = typename std::enable_if<std::is_convertible<Callable, OperationCompleteFunction>::value>::type>
	int add_listener(Callable&& callee) {
		std::lock_guard<std::mutex> lk(mutex_);
		const FutureState id = EventTrigger::bind<OperationCompleteFunction>(kComplete, std::forward<Callable>(callee));
		listener_handlers_.push_back(id);
		if (is_done()) {
			notify_listeners();
		}
		return id;
	}

	template <typename CallableHint, typename Callable, typename ...Args,
			typename = typename std::enable_if<std::is_convertible<Callable, OperationCompleteFunction>::value>::type>
	int add_listener(Callable&& callee, Args&& ...args) {
		std::lock_guard<std::mutex> lk(mutex_);
		const FutureState id =
			EventTrigger::bind<std::function<void>>(kComplete, std::bind(callee, std::forward<Args>(args)...));
		listener_handlers_.push_back(id);
		if (is_done()) {
			notify_listeners();
		}
		return id;
	}

	void remove_listener(const FutureState id) {
		std::lock_guard<std::mutex> lk(mutex_);
		typename  ListenerHandlerVector::iterator it = std::find(listener_handlers_.begin(), listener_handlers_.end(), id);
		if (it != listener_handlers_.end()) {
			listener_handlers_.erase(id);
			EventTrigger::unbind(id);
		}
	}


private:

	std::mutex mutex_;
	std::condition_variable cond_;
	ListenerHandlerVector listener_handlers_;


};

}

#endif
