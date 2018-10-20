//
// Created by finefenng on 2018/9/22.
//

#include <pump/utility/log/AppendFile.h>

namespace pump {
namespace utility {

AppendFile::AppendFile(const std::string& file_name)
	: file_(::fopen(file_name.c_str(), "a")) {
  assert(file_);
  setbuffer(file_, buffer_, sizeof(buffer_));
}

AppendFile::~AppendFile() {
  fclose(file_);
}

void AppendFile::append(const pump::BYTE_T* log_line, size_t length) {
  size_t count = write(log_line, length);
  size_t remain = length - count;
  while (remain > 0) {
	size_t xx = write(log_line + count, remain);
	if (xx == 0) {
	  //TODO
	  break;
	}
	count += xx;
	remain -= xx;
  }
  written_bytes_ += length;
}

size_t AppendFile::write(const pump::BYTE_T* log_line, size_t length) {
  return ::fwrite(log_line, 1, length, file_);
}



} // utility
} // pump

