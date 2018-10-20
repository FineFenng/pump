//
// Created by finefenng on 2018/9/21.
//

#include <pump/utility/log/FileLogger.h>

namespace pump {

namespace utility {

namespace /*anonymous*/ {

const int kRollPerSeconds = 60 * 60 * 24;

} //anonymous

void FileLogger::append(const BYTE_T* log_line, size_t length) {
  if (is_thread_safe_) {
    std::lock_guard<std::mutex> lk(mutex_);
    append_unlocked(log_line, length);
  } else {
    append_unlocked(log_line, length);
  }
}

void FileLogger::flush() {
  if (is_thread_safe_) {
    std::lock_guard<std::mutex> lk(mutex_);
    file_->flush();
  } else {
    file_->flush();
  }
}

void FileLogger::append_unlocked(const BYTE_T* log_line, size_t length) {
  file_->append(log_line, length);
  if (file_->get_written_bytes() > roll_size_) {
    roll_file();
  } else {
    ++count_;
    if (count_ >= check_every_count_) {
      count_ = 0;
      time_t now = time(nullptr);
      time_t this_period = now / kRollPerSeconds * kRollPerSeconds;
      if (this_period > start_) {
        roll_file();
      } else if (now - last_flush_ > flush_interval_) {
        last_flush_ = now;
        file_->flush();
      }
    }
  }
}

bool FileLogger::roll_file() {
  time_t now = 0;
  std::string file_name = get_log_file_name(base_name_, &now);
  time_t start = now / kRollPerSeconds * kRollPerSeconds;

  if (now > last_roll_) {
    last_roll_ = now;
    last_flush_ = now;
    start_ = start;
    file_.reset(new AppendFile(file_name));
    return true;
  }

  return false;
}



std::string utility::FileLogger::get_log_file_name(const std::string& base_name, time_t* now) {

  std::string file_name;
  file_name.reserve(base_name.size() + 64);
  file_name = base_name;

  char time_buff[32];
  struct tm tm;
  *now = time(nullptr);
  gmtime_r(now, &tm);
  strftime(time_buff, sizeof(time_buff), ".%Y%m%d-%H%M%S", &tm);
  file_name + time_buff;

  int32_t current_pid;
#ifdef PUMP_PLATFORM_GNU
  current_pid = getpid();
#endif

#ifdef PUMP_PLATFORM_WIN
  current_pid = GetCurrentProcessId()
#endif

  file_name += std::to_string(current_pid);
  file_name += ".log";

  return file_name;

}

} //utility
} //pump



