//
// Created by finefenng on 2018/9/21.
//

#ifndef PUMP_UTILITY_LOGFILE_H
#define PUMP_UTILITY_LOGFILE_H

#include <string>

#include <pump/Common.h>

namespace pump {
namespace utility {

class LogFile {
 public:
  LogFile(const std::string& base_name, size_t roll_size, int flush_interval = 3, int check_every_count = 1024)
	  : base_name_(base_name),
		roll_size_(roll_size),
		flush_interval_(flush_interval),
		check_every_count_(check_every_count) {



  }

  ~LogFile() {

  }

  void append(const BYTE_T* log_line, size_t length);

  void flush();

  void roll_file();

 PUMP_DECLARE_DEFAULT_COPYABLE(LogFile);
 PUMP_DECLARE_DEFAULT_MOVABLE(LogFile);

 private:

  std::string get_log_file_name(const std::string& base_name, time_t* now);
  std::string base_name_;
  size_t roll_size_;

  const int flush_interval_;
  const int check_every_count_;
};

}

}

#endif //PUMP_LOGFILE_H
