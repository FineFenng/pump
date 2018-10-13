#ifndef PUMP_UTILITY_FIXEDBUFFER_H
#define PUMP_UTILITY_FIXEDBUFFER_H

#include <pump/Common.h>

namespace pump { namespace utility {
template<unsigned int SIZE>
class FixedBuffer {
 public:
  FixedBuffer()
	  : data_{0},
		cur_(data_) {}

  void append(const char* buf, size_t len) {
	if (avail() > len) {
	  memcpy(cur_, buf, len);
	  cur_ += len;
	}
  }

  const char* data() const { return data_; }

  char* current() const { return cur_; }

  size_t length() const { return cur_ - data_; }

  void add(size_t len) { cur_ += len; }

  void reset() { cur_ = data_; }

  void bzero() { memset(data_, 0, sizeof(data_)); }

  size_t avail() const { return end() - cur_; }

 private:

  const char* end() const { return data_ + sizeof(data_); }

  char data_[SIZE];
  char* cur_;
};
static const int kSmallBuffer = 4000;
static const int kLargeBuffer = 4000 * 1000;

typedef FixedBuffer<kSmallBuffer> SmallFixedBuffer;
typedef FixedBuffer<kLargeBuffer> LargeFixedBuffer;

}}

#endif
