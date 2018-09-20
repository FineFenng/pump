//
// Created by finefenng on 2018/9/16.
//

#ifndef PUMP_SINGLETON_H
#define PUMP_SINGLETON_H

#include <atomic>
#include <mutex>

#include <pump/Common.h>


namespace pump {

template<typename T>
class Singleton {
public:

	Singleton() = delete;
	~Singleton() = delete;
PUMP_DECLARE_DELETE_COPYABLE(Singleton)
PUMP_DECLARE_DELETE_MOVABLE(Singleton)

	static T* get_instance()
	{
		T* re = instance_.load(std::memory_order_acquire);

		if (re == nullptr) {
			static std::mutex           _mutex_;
			std::lock_guard<std::mutex> lk(_mutex_);

			re = instance_.load(std::memory_order_acquire);
			if (re == nullptr) {
				re = new T;
				instance_.store(re, std::memory_order_release);
			}
		}
		return re;
	}

	static void destory_instance()
	{
		T* re =

	}


private:
	static std::atomic<T*> instance_;
};

template<typename T> std::atomic<T*> Singleton<T>::instance_ = nullptr;

}



#endif //PUMP_SINGLETON_H
