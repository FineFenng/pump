//
// Created by finefenng on 2018/9/22.
//

#ifndef PUMP_UTILITY_APPENDFILE_H
#define PUMP_UTILITY_APPENDFILE_H

#include <cstdio>

#include <string>

#include <pump/Common.h>

namespace pump {
namespace utility {

class AppendFile {
 public:
  explicit AppendFile(const std::string& file_name);
  ~AppendFile();

  void append(const BYTE_T* log_line, size_t length);

  void flush() { ::fflush(file_); }

  size_t get_written_bytes() { return written_bytes_; }

 private:

  size_t write(const BYTE_T* log_line, size_t length);

  FILE* file_;
  char buffer_[64 * 1024];
  size_t written_bytes_;

};

}

}

#endif //PUMP_APPENDFILE_H
