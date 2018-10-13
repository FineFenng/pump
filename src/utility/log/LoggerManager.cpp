//
// Created by finefenng on 2018/9/30.
//

#include <pump/utility/log/LoggerManager.h>

namespace pump {
namespace utility {

LoggerManager::LoggerManager()
	: is_inited_(false), console_logger_() {
  init();
}
void LoggerManager::register_logger(const std::weak_ptr<LoggerInterface>& logger) {
  std::lock_guard<std::mutex> lk(mutex_);
  loggers_.push_back(logger);
}
void LoggerManager::remove_logger(const LoggerInterface* logger) {
  std::lock_guard<std::mutex> lk(mutex_);
  auto it = loggers_.begin();
  while (it != loggers_.end()) {
	if (it->lock().get() == logger) {
	  it = loggers_.erase(it);
	} else {
	  ++it;
	}
  }
}

}
}

