// Created by finefenng on 2017/12/1.
//

#ifndef PUMP_UTILITY_LOGSTREAM_H
#define PUMP_UTILITY_LOGSTREAM_H


#include <cstddef>
#include <cstring>

#include <memory>
#include <string>

#include <pump/utility/SourceFile.h>

namespace pump { namespace utility
{
static const int kSmallBuffer = 4000;
static const int kLargeBuffer = 4000 * 1000;

static const char digits[] = {
	'9', '8', '7', '6', '5', '4', '3', '2', '1',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

static const char* zero = digits + 9;

template <typename T>
inline size_t ConvertNumToString(const T& t, char* buffer)
{
	T i = t;
	char* p = buffer;

	do {
		const int lsd = static_cast<int>(i % 10);
		i /= 10;
		*p++ = zero[lsd];
	}
	while (i != 0);

	if (t < 0) {
		*p++ = '-';
	}

	*p = '\0';

	std::reverse(buffer, p);
	return p - buffer;
}

template <unsigned int SiZE>
class FixedBuffer
{
public:
	FixedBuffer()
		: data_{0}, cur_(data_)
	{
	}


	void append(const char* buf, size_t len)
	{
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

	char data_[SiZE];
	char* cur_;
};

class LogStream
{
public:
	typedef LogStream self;
	typedef FixedBuffer<kSmallBuffer> LogBuffer;

	LogStream() = default;

	self& operator<<(bool val);
	self& operator<<(unsigned short val);
	self& operator<<(short val);
	self& operator<<(unsigned int val);
	self& operator<<(int val);
	self& operator<<(unsigned long val);
	self& operator<<(long val);
	self& operator<<(unsigned long long val);
	self& operator<<(long long val);
	self& operator<<(void* val);

	self& operator<<(float val);
	self& operator<<(double val);

	self& operator<<(char ch);

	template<unsigned int N>
	self& operator<<(const char (&str)[N])
	{
		return operator<<(str);
	}

	self& operator<<(const char* str);
	self& operator<<(const std::string& str);
	self& operator<<(const SourceFile& source_file);

	void append(const char* data, int len);

	const LogBuffer& buffer() const;

	void resetBuffer();

private:
	static const size_t max_numeric_size = 32;

	template <typename T>
	inline void format_integet(T v)
	{
		if (buffer().avail() > max_numeric_size) {
			size_t len = ConvertNumToString(v, buffer_.current());
			buffer_.add(len);
		}
	}


private:
	LogBuffer buffer_;
};
}}


#endif //LOGGER_LOGSTREAM_H
