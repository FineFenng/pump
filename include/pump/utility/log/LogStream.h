// Created by finefenng on 2017/12/1.
//

#ifndef PUMP_UTILITY_LOGSTREAM_H
#define PUMP_UTILITY_LOGSTREAM_H


#include <cstddef>
#include <cstring>

#include <algorithm>
#include <memory>
#include <string>

#include <pump/utility/FixedBuffer.h>
#include <pump/utility/SourceFile.h>


namespace pump { namespace utility
{
;
static const char digits[] = {
	'F', 'E', 'D', 'C', 'B', 'A', '9', '8', '7', '6', '5', '4', '3', '2', '1',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};


template <typename T>
inline size_t ConvertNumToString(const T& t, char* buffer, int base)
{
	T i = t;
	char* p = buffer;

	const char* const zero = digits + 15;

	do {
		const auto lsd = static_cast<int>(i % base);
		i /= base;
		*p++ = zero[lsd];
	} while (i != 0);

	if (t < 0) {
		*p++ = '-';
	}

	*p = '\0';

	std::reverse(buffer, p);
	return p - buffer;
}


class LogStream
{
public:
	typedef LogStream self;
	typedef SmallFixedBuffer LogBuffer;

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

	template <unsigned int N>
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
	inline void format_integer(T v)
	{
		if (buffer().avail() > max_numeric_size) {
			const size_t len = ConvertNumToString(v, buffer_.current(), 10);
			buffer_.add(len);
		}
	}


private:
	LogBuffer buffer_;
};
}}


#endif
