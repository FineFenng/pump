#ifndef PUMP_PACKAGE_H_
#define PUMP_PACKAGE_H_
#include <cstdint>
#include <cstdlib>
#include <pump/Common.h>
#include <pump/BytesHelper.h>


#define PACK_MALLOC_H_RESERVER  WAWO_MALLOC_H_RESERVE

#define PACK_MIN_LEFT_CAPACITY (128)
#define PACK_MAX_LEFT_CAPACITY (1024*1024*8)
#define PACK_MIN_RIGHT_CAPACITY (256)
#define PACK_MAX_RIGHT_CAPACITY (1024*1024*64)
#define PACK_MIN_CAPACITY (PACK_MIN_LEFT_CAPACITY + PACK_MIN_RIGHT_CAPACITY)
#define PACK_MAX_CAPACITY (PACK_MAX_LEFT_CAPACITY + PACK_MAX_RIGHT_CAPACITY)
#define PACK_INIT_LEFT_CAPACITY ((PACK_MIN_LEFT_CAPACITY))
#define PACK_INIT_RIGHT_CAPACITY (((1024*4)-PACK_MIN_LEFT_CAPACITY)-PACK_MALLOC_H_RESERVER)
#define PACK_INCREMENT_LEFT_SIZE ((1024*8)-PACK_MALLOC_H_RESERVER)
#define PACK_INCREMENT_RIGHT_SIZE ((1024*8)-PACK_MALLOC_H_RESERVER)


namespace pump
{
class Packet
{
public:
	Packet(uint32_t left_capacity = PACK_INIT_LEFT_CAPACITY, uint32_t right_capacity = PACK_INIT_RIGHT_CAPACITY)
		: buffer_(nullptr),
		left_capacity_(left_capacity),
		right_capacity_(right_capacity),
		read_index_(0),
		write_index_(0)
	{
		init_buffer(left_capacity_, right_capacity_);
	}

	explicit Packet(const BYTE_T* buffer, uint32_t len)
		: buffer_(nullptr),
		left_capacity_(PACK_INIT_LEFT_CAPACITY),
		right_capacity_(0),
		read_index_(0),
		write_index_(0)
	{
		init_buffer(left_capacity_, right_capacity_);
		write(buffer, len);
	}

	~Packet()
	{
		free(buffer_);
	}


public:
	void init_buffer(uint32_t left, uint32_t right)
	{
		left_capacity_ = PUMP_MAX2(left, PACK_MIN_LEFT_CAPACITY);
		right_capacity_ = PUMP_MAX2(right, PACK_MIN_RIGHT_CAPACITY);
		reset();
		buffer_ = static_cast<BYTE_T*>(malloc(sizeof(BYTE_T) * capacity()));
	}


	void extend_left_buffer_capacity()
	{
		left_capacity_ += PACK_INCREMENT_LEFT_SIZE;
		auto* const new_buffer = static_cast<BYTE_T*>(realloc(buffer_, capacity()));
		buffer_ = new_buffer;
		const uint32_t new_left = read_index_ + PACK_INCREMENT_LEFT_SIZE;
		const uint32_t len = get_len();

		if (len > 0) {
			memmove(buffer_ + new_left, buffer_ + read_index_, len);
		}
		read_index_ = new_left;
		write_index_ = read_index_ + len;
	}

	void extend_right_buffer_capacity()
	{
		right_capacity_ += PACK_INCREMENT_RIGHT_SIZE;
		auto* const new_buffer = static_cast<BYTE_T*>(realloc(buffer_, capacity()));
		buffer_ = new_buffer;
	}

	const BYTE_T* begin() const
	{
		return buffer_ + read_index_;
	}

	const BYTE_T* end() const
	{
		return buffer_ + write_index_;
	}

	void forward_write_index(const uint32_t& bytes)
	{
		write_index_ += bytes;
	}

	Packet* write_left(const BYTE_T* buffer, uint32_t len)
	{
		while (len > (left_left_capacity())) {
			extend_left_buffer_capacity();
		}
		read_index_ -= len;
		::memcpy(buffer_ + read_index_, buffer, len);
		return this;
	}

	template <typename T>
	Packet* write_left(const T& t)
	{
		uint32_t to_write_length = sizeof(t);
		while (to_write_length > left_left_capacity()) {
			extend_left_buffer_capacity();
		}
		read_index_ -= sizeof(t);
		uint32_t wrote_bytes = pump::bigendian::Write_xx_impl(t, buffer_ + read_index_);
		return this;
	}


	template <typename T>
	T read()
	{
		const T t = pump::bigendian::Read_XX_Impl(buffer_ + read_index_, pump::bigendian::Type<T>());
		read_index_ += sizeof(T);
		return t;
	}

	uint32_t read(uint32_t len, BYTE_T* target)
	{
		if (target == nullptr || len == 0) { return 0; }
		const uint32_t c = (get_len() > len ? len : get_len());
		memcpy(target, buffer_ + read_index_, c);
		read_index_ += c;
		return c;
	}


	template <typename T>
	Packet* write(const T& t)
	{
		uint32_t val_len = sizeof(t);
		while (val_len > left_left_capacity()) {
			extend_right_buffer_capacity();
		}
		write_index_ += pump::bigendian::Write_xx_impl(t, (buffer_ + write_index_));
		return this;
	}

	Packet* write(const BYTE_T* buffer, uint32_t len)
	{
		while (len > left_left_capacity()) {
			extend_right_buffer_capacity();
		}
		memcpy(buffer_ + write_index_, buffer, len);
        BYTE_T* pos = buffer_ + write_index_;
        

		write_index_ += len;
		return this;
	}

	template <typename T>
	T peek() const
	{
		T t = pump::bigendian::Read_XX_Impl(buffer_ + read_index_, pump::bigendian::Type<T>());
		return t;
	}

	uint32_t peek(size_t len, BYTE_T* buffer) const
	{
		if (buffer == nullptr || len == 0) { return 0; }

		const uint32_t can_peek_size = len > get_len() ? get_len() : len;
		memcpy(buffer, buffer_ + read_index_, can_peek_size);
		return can_peek_size;
	}

	uint32_t get_len() const { return write_index_ - read_index_; }

	void reset() { read_index_ = write_index_ = left_capacity_; }

private:


	uint32_t capacity() const { return left_capacity_ + right_capacity_; }

	uint32_t left_left_capacity() const { return buffer_ == nullptr ? 0 : read_index_; }
	uint32_t right_left_capacity() const { return buffer_ == nullptr ? 0 : capacity() - write_index_; }

private:
	BYTE_T* buffer_;
	uint32_t left_capacity_;
	uint32_t right_capacity_;
	uint32_t read_index_;
	uint32_t write_index_;
};
}
#endif
