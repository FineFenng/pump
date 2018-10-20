//
// Created by finefenng on 2018/9/21.
//

#ifndef PUMP_UTILITY_LOGFILE_H
#define PUMP_UTILITY_LOGFILE_H

#include <mutex>
#include <string>

#include <pump/Common.h>
#include <pump/utility/log/LoggerInterface.h>
#include <pump/utility/log/AppendFile.h>

namespace pump {
namespace utility {

class FileLogger : public LoggerInterface {
 PUMP_DECLARE_DELETE_COPYABLE_AND_MOVABLE(FileLogger)
 public:
  FileLogger(const std::string& base_name, size_t roll_size,
		  int flush_interval = 3, int check_every_count = 1024, bool is_thread_safe = false)
	  : base_name_(base_name),
		roll_size_(roll_size),
		file_(),
		flush_interval_(flush_interval),
		check_every_count_(check_every_count),
		is_thread_safe_(is_thread_safe) {

	roll_file();

  }

  ~FileLogger() {

  }

  void append(const BYTE_T* log_line, size_t length);


  void flush();

  bool roll_file();

 private:
  void append_unlocked(const BYTE_T* log_line, size_t length);
  std::string get_log_file_name(const std::string& base_name, time_t* now);

 private:
  std::string base_name_;
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


};

}

}

#endif //PUMP_LOGFILE_H
