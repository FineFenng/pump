#include <pump/utility/log/LogStream.h>
#include <sstream>


namespace pump { namespace utility
{
LogStream::self& LogStream::operator<<(bool val)
{
    char temp = val ? '1' : '0';
    buffer_.append(&temp, 1);
    return *this;
}


LogStream::self& LogStream::operator<<(unsigned short val)
{
    return operator<<(static_cast<unsigned int>(val));
}

LogStream::self& LogStream::operator<<(short val)
{
    return operator<<(static_cast<int>(val));
}

LogStream::self& LogStream::operator<<(unsigned int val)
{
    format_integet(val);
    return *this;
}

LogStream::self& LogStream::operator<<(int val)
{
    format_integet(val);
    return *this;
}

LogStream::self& LogStream::operator<<(unsigned long val)
{
    format_integet(val);
    return *this;
}

LogStream::self& LogStream::operator<<(long val)
{
    format_integet(val);
    return *this;
}
LogStream::self& LogStream::operator<<(void* val)
{
	return operator<<(reinterpret_cast<unsigned long long>(val));
}

LogStream::self& LogStream::operator<<(unsigned long long val)
{
    format_integet(val);
    return *this;
}

LogStream::self& LogStream::operator<<(long long val)
{
    format_integet(val);
    return *this;
}

LogStream::self& LogStream::operator<<(float val)
{
    return operator<<(static_cast<double>(val));
}

LogStream::self& LogStream::operator<<(double val)
{
    if (buffer_.avail() >= max_numeric_size) {
        int len = snprintf(buffer_.current(), max_numeric_size, "%.12g", val);
        buffer_.add(static_cast<size_t>(len));
    }
    return *this;
}

LogStream::self& LogStream::operator<<(char ch)
{
    buffer_.append(&ch, 1);
    return *this;
}

LogStream::self& LogStream::operator<<(const char* str)
{
    size_t len = strlen(str);
    buffer_.append(str, len);
    return *this;
}

LogStream::self& LogStream::operator<<(const std::string& str)
{
    return operator<<(str.c_str());
}

LogStream::self& LogStream::operator<<(const SourceFile& source_file)
{
    return operator<<(source_file.data());
}

void LogStream::append(const char* str, int len)
{
    buffer_.append(str, static_cast<size_t>(len));
}


const LogStream::LogBuffer& LogStream::buffer() const
{
    return buffer_;
}

void LogStream::resetBuffer()
{
    buffer_.reset();
    buffer_.bzero();
}

}}
