//
// Created by finefenng on 2018/9/21.
//

#ifndef PUMP_UTILITY_LOGFILE_H
#define PUMP_UTILITY_LOGFILE_H

#include <mutex>
#include <string>
#include <syslog.h>

#include <pump/Common.h>
#include <pump/utility/log/LoggerInterface.h>
#include <pump/utility/log/AppendFile.h>

namespace pump {
namespace utility {

#define PUMP_IDENT_LENGTH 32

class SystemLogger : public LoggerInterface {
 PUMP_DECLARE_DELETE_COPYABLE_AND_MOVABLE(SystemLogger)
 public:
  SystemLogger(const char* const ident, size_t roll_size,
               int flush_interval = 3, int check_every_count = 1024, bool is_thread_safe = false)
      : roll_size_(roll_size),
        file_(),
        flush_interval_(flush_interval),
        check_every_count_(check_every_count),
        is_thread_safe_(is_thread_safe) {
    strncpy(ident_, ident, PUMP_IDENT_LENGTH - 1);
    printf("[syslog]syslog init begin\n");
    openlog(ident_, LOG_PID, LOG_LOCAL0);
    printf("[syslog]syslog init ok, ident name: %s", ident_);
    syslog(LOG_INFO, "[syslog]syslog init");
    roll_file();
  }

  ~SystemLogger() {

  }

  void append(const BYTE_T* log_line, size_t length);

  void flush();

  bool roll_file();

 private:
  void append_unlocked(const BYTE_T* log_line, size_t length);

 private:
  char ident_[PUMP_IDENT_LENGTH];
  size_t roll_size_;
  int count_;
  std::unique_ptr<AppendFile> file_;
  const int flush_interval_;
  const int check_every_count_;
  time_t start_;
  time_t last_roll_;
  time_t last_flush_;
  bool is_thread_safe_;
  std::mutex mutex_;

}; //

}// utility

}// pump

#endif //PUMP_LOGFILE_H
