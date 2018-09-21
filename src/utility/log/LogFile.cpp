//
// Created by finefenng on 2018/9/21.
//

#include <pump/utility/log/LogFile.h>

namespace pump {

namespace utility {

namespace {

const int kRollPerSeconds = 60 * 60 * 24;
}

void LogFile::roll_file() {
  time_t now = 0;
  std::string file_name = get_log_file_name(base_name_, &now);


}

std::string utility::LogFile::get_log_file_name(const std::string& base_name, time_t* now) {

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

}

}



