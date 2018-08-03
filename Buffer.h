//
// Created by fenng on 18-8-3.
//

#ifndef QPSTEST_BUFFER_H
#define QPSTEST_BUFFER_H


/*     Netty Buffer Design
*      +-------------------+------------------+------------------+
*      | discardable bytes |  readable bytes  |  writable bytes  |
*      |                   |     (CONTENT)    |                  |
*      +-------------------+------------------+------------------+
*      |                   |                  |                  |
*      0      <=      readerIndex   <=   writerIndex    <=    capacity
*/

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

class Buffer
{
public:
    typedef std::vector<unsigned char>::iterator BufferIterator;
    typedef std::vector<unsigned char>::const_iterator C_BufferIterator;
public:
    Buffer()
    : data_(k_init_buffer_size_),
    readable_index_(0), writable_index_(0), buffer_capacity_(k_init_buffer_size_)
    {}

    ~Buffer() = default;

    void append(const std::string& message)
    {
        append(message.c_str(), message.size());
    }

    void append(const char* message, size_t len)
    {
        ensure_capacity(len);
        std::memcpy(&*get_end(), message, len);
        writable_index_ += len;
    }

    void append_from_fd(int fd);

    void retrieve(size_t len) { readable_index_ += len; }

    void retrieve_all() { readable_index_ = writable_index_ = 0; }

    void retrieve_position(const unsigned char* pos) { retrieve(pos - &*get_begin()); }

    std::string retrieve_all_into_string()
    {
        std::string str(get_begin(), get_end());
        retrieve_all();
        return str;
    }


private:


    size_t get_readable_index() { return readable_index_; }
    size_t get_writable_index() { return writable_index_; }

    size_t get_readable_bytes() { return writable_index_ - readable_index_; }
    size_t get_writable_bytes() { return buffer_capacity_ - writable_index_; }

    BufferIterator get_begin() { return data_.begin(); }
    BufferIterator get_end() { return data_.begin() + writable_index_; }

    size_t get_buffer_capacity() { return buffer_capacity_; }
    size_t set_buffer_capacity(size_t capacity) { buffer_capacity_ = capacity; }


    void ensure_capacity(size_t byte_count)
    {
        if (get_writable_bytes() < byte_count)
        {
            data_.resize(writable_index_ + byte_count);
            set_buffer_capacity(data_.capacity());
        }
        else
        {
            std::copy(get_begin() + readable_index_, get_begin() + writable_index_, get_begin());
            size_t bytes_record = get_readable_bytes();
            readable_index_ = 0;
            writable_index_ = readable_index_ +  bytes_record;
        }
    };


private:
    std::vector<unsigned char> data_;
    size_t readable_index_;
    size_t writable_index_;
    size_t buffer_capacity_;

    static const unsigned int k_init_buffer_size_ = 1024;

};


#endif //QPSTEST_BUFFER_H
