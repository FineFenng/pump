//
// Created by finefenng on 2018/9/21.
//

#include <pump/utility/log/ConsoleLogger.h>

namespace pump {

namespace utility {

namespace /*anonymous*/ {

const int kRollPerSeconds = 60 * 60 * 24;

}

void ConsoleLogger::append(const BYTE_T* log_line, size_t length) {
  if (is_thread_safe_) {
    std::lock_guard<std::mutex> lk(mutex_);
    append_unlocked(log_line, length);
  } else {
    append_unlocked(log_line, length);
  }
}

void ConsoleLogger::flush() {
  if (is_thread_safe_) {
    std::lock_guard<std::mutex> lk(mutex_);
    file_->flush();
  } else {
    file_->flush();
  }
}

void ConsoleLogger::append_unlocked(const BYTE_T* log_line, size_t length) {
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

bool ConsoleLogger::roll_file() {
  time_t now = 0;
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



}
}



