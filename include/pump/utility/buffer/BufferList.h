#ifndef PUMP_UTILIY_BUFFERLIST_H
#define PUMP_UTILIY_BUFFERLIST_H

#include <string>

#include <pump/Common.h>
#include <pump/utility/FixedBuffer.h>

namespace pump { namespace utility
{
class BufferList
{
public:

	void append_string(const char* message, size_t len);

	void append_string(const void* message, size_t len);

	void append_string(const std::string& message);

	void append_uint32(uint32_t value)
	{
		
	}

	void recv_from_fd(SOCKET fd, int* saved_errno);

	void retrieve(size_t len)
	{
		readable_index_ += len;
		PUMP_ASSERT(readable_index_ <= writable_index_);
	}

	void retrieve_all() { readable_index_ = writable_index_ = 0; }

	void retrieve_position(const BYTE_T* pos)
	{ }

	std::string retrieve_all_to_string()
	{

		
	}


	size_t get_readable_bytes() const { return writable_index_ - readable_index_; }
	size_t get_writable_bytes() const { return small_buffer_capacity_ - final_buffer_writable_index_; }

	BYTE_T* get_readable_end()
	{
		return data_.data() + writable_index_;
	}
	const BYTE_T* get_readable_end() const { return data_.data() + writable_index_; }

	BYTE_T* get_readable_address() { return data_.data() + readable_index_; }
	const BYTE_T* get_readable_address() const { return data_.data() + readable_index_; }

	BYTE_T* get_writable_end() { return data_.data() + buffer_capacity_; }
	const BYTE_T* get_writable_end() const { return data_.data() + buffer_capacity_; }

	BYTE_T* get_writable_address() { return data_.data() + writable_index_; }
	const BYTE_T* get_writable_address() const { return data_.data() + writable_index_; }






private:
	SmallFixedBuffer* head;
	SmallFixedBuffer* tail;
	size_t buffer_count_;
	size_t small_buffer_capacity_;
	

private:
	size_t readable_index_;
	size_t writable_index_;
	size_t final_buffer_writable_index_;
};
}}


#endif
