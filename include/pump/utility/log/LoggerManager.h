//
// Created by finefenng on 2018/9/30.
//

#ifndef PUMP_UTILITY_LOGMANAGER_H
#define PUMP_UTILITY_LOGMANAGER_H

#include <memory>
#include <mutex>
#include <vector>

#include <pump/Singleton.h>
#include <pump/utility/log/LoggerInterface.h>
#include <pump/utility/thread/SpinLock.h>
#include <pump/utility/log/AsyncLogging.h>

namespace pump {
namespace utility {

class SpinLock;

class LoggerManager : public pump::Singleton<LoggerManager> {
 public:

  void init() {
    if (is_inited_) {
      return;
    }
    is_inited_ = true;
  }

  void deinit() {
    loggers_.clear();
  }

  void register_logger(const std::weak_ptr<LoggerInterface>& logger);

  void remove_logger(const LoggerInterface* logger);

  void write_into_logger(const BYTE_T* log_line, size_t len) {
    spinLock_.lock();
    for (auto it = loggers_.begin(); it != loggers_.end(); ++it) {
      it->lock()->append(log_line, len);
    }
    spinLock_.unlock();
  }
 protected:
  LoggerManager();
  ~LoggerManager() = default;

 private:
  bool is_inited_;
  std::mutex mutex_;
  SpinLock spinLock_;
  std::vector<std::weak_ptr<LoggerInterface>> loggers_;
  AsyncLogging async_logging_;
};

}
}

#endif //PUMP_LOGMANAGER_H
