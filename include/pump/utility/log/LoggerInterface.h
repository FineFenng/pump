//
// Created by finefenng on 2018/9/30.
//

#ifndef PUMP_UTILITY_LOGGERBASE_H
#define PUMP_UTILITY_LOGGERBASE_H

#include <string>

#include <pump/Common.h>

namespace pump {
namespace utility {

class LoggerInterface {
 public:

  virtual ~LoggerInterface() = default;

  virtual void append(const BYTE_T* log_line, size_t length) = 0;

  virtual void flush() = 0;

  virtual bool roll_file() = 0;

 private:
  virtual void append_unlocked(const BYTE_T* log_line, size_t length) = 0;

};

bool operator==(const std::reference_wrapper<LoggerInterface>& lhs,
				const std::reference_wrapper<LoggerInterface>& rhs) {
  return &lhs.get() == &rhs.get();
}

}
}

#endif //PUMP_LOGGERINTERFACE_H
