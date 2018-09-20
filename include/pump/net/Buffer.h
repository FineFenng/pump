//
// Created by fenng on 18-8-3.
//

#ifndef PUMP_NET_BUFFER_H_
#define PUMP_NET_BUFFER_H_


/*     Netty Buffer Design
*      +-------------------+------------------+------------------+
*      | discardable bytes |  readable bytes  |  writable bytes  |
*      |                   |     (CONTENT)    |                  |
*      +-------------------+------------------+------------------+
*      |                   |                  |                  |
*      0      <=      readerIndex   <=   writerIndex    <=    capacity
*/


// reply on os header file


// standard c header file
#include <cassert>
#include <cstring>

// standard c++  header file
#include <algorithm>
#include <string>
#include <vector>

// own define header file
#include <pump/Common.h>
#include <pump/BytesHelper.h>

namespace pump { namespace net
{
class Buffer
{
public:
	Buffer()
		: data_(kInitBufferSize_),
		readable_index_(0),
		writable_index_(0),
		buffer_capacity_(kInitBufferSize_)
	{ }



	void append_string(const char* message, size_t len)
	{
		ensure_capacity(len);
		memcpy(get_writable_address(), message, len);
		writable_index_ += len;
	}

	void append_string(const void* message, size_t len)
	{
		append_string(static_cast<const char*>(message), len);
	}

	void append_string(const std::string& message)
	{
		append_string(message.c_str(), message.size());
	}

	void append_uint32(uint32_t value)
	{
		uint32_t wrote_count = pump::bigendian::Write_uint32(value, get_writable_address());
	}


	int recv_from_fd(SOCKET fd, int* saved_errno);

	void retrieve(size_t len)
	{
		readable_index_ += len;
		PUMP_ASSERT(readable_index_ <= writable_index_);
	}

	void retrieve_all() { readable_index_ = writable_index_ = 0; }

	void retrieve_position(const BYTE_T* pos) { retrieve(pos - get_readable_address()); }

	std::string retrieve_all_to_string()
	{
		std::string str(get_readable_address(), get_readable_end());
		retrieve_all();
		return str;
	}

	size_t get_readable_bytes() const { return writable_index_ - readable_index_; }
	size_t get_writable_bytes() const { return buffer_capacity_ - writable_index_; }

	BYTE_T* get_readable_end() { return data_.data() + writable_index_; }
	const BYTE_T* get_readable_end() const { return data_.data() + writable_index_; }

	BYTE_T* get_readable_address() { return data_.data() + readable_index_; }
	const BYTE_T* get_readable_address() const { return data_.data() + readable_index_; }

	BYTE_T* get_writable_end() { return data_.data() + buffer_capacity_; }
	const BYTE_T* get_writable_end() const { return data_.data() + buffer_capacity_; }

	BYTE_T* get_writable_address() { return data_.data() + writable_index_; }
	const BYTE_T* get_writable_address() const { return data_.data() + writable_index_; }

	void ensure_capacity(size_t byte_count)
	{
		if (get_writable_bytes() < byte_count) {
			data_.resize(writable_index_ + byte_count);
			set_buffer_capacity(data_.capacity());
		}
		else {
			std::copy(get_readable_address() + readable_index_, get_readable_address() + writable_index_,
					get_readable_address());
			const size_t bytes_record = get_readable_bytes();
			readable_index_ = 0;
			writable_index_ = readable_index_ + bytes_record;
		}
	}

private:
	void set_buffer_capacity(size_t capacity) { buffer_capacity_ = capacity; }

private:
	std::vector<BYTE_T> data_;

	size_t readable_index_;
	size_t writable_index_;
	size_t buffer_capacity_;

	static const unsigned int kInitBufferSize_ = 1024;
};
}}
#endif 
