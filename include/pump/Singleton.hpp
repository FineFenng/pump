//
// Created by finefenng on 2018/9/16.
//

#ifndef PUMP_SINGLETON_H
#define PUMP_SINGLETON_H

#include <cstdlib>

#include <atomic>
#include <mutex>


#include <pump/Common.h>

namespace pump {

template<typename T>
class Singleton {
 PUMP_DECLARE_DELETE_COPYABLE_AND_MOVABLE(Singleton)

 public:
  static T* get_instance() {
    T* re = instance_.load(std::memory_order_acquire);

    if (re == nullptr) {
      static std::mutex constructor_mutex;
      std::lock_guard<std::mutex> lk(constructor_mutex);

      re = instance_.load(std::memory_order_acquire);
      if (re == nullptr) {
        re = new T();
        schedule_for_destroy(Singleton<T>::destroy_instance);
        instance_.store(re, std::memory_order_release);
      }
    }
    return re;
  }

  static void destroy_instance() {
    T* re = instance_.load(std::memory_order_acquire);
    if (re != nullptr) {
      static std::mutex destroy_mutex;
      std::lock_guard<std::mutex> lk(destroy_mutex);
      if (re != nullptr) {
        delete re;
        instance_.store(nullptr, std::memory_order_release);
      }
    }
  }

 protected:
  Singleton() {}
  virtual ~Singleton() {};

 protected:
  static void schedule_for_destroy(void(* func)()) {
    std::atexit(func);
  }

 private:
  static std::atomic<T*> instance_;
};

template<typename T> std::atomic<T*> Singleton<T>::instance_(nullptr);

}

#endif //PUMP_SINGLETON_H
