#ifndef PUMP_BYTEHELPER_H_
#define PUMP_BYTEHELPER_H_

#include <cstdint>

#include <utility>

namespace pump {namespace bigendian
{
template <typename T>
struct Type
{
};

template <typename T, typename OrigiPtr>
inline T Read_XX_Impl(const OrigiPtr& start, Type<T>)
{
	T ret = 0;
	for (size_t i = 0; i < sizeof(T); i++) {
		ret <<= 8;
		ret |= static_cast<uint8_t>(*(start + i));
	}

	return ret;
}


/*
template<typename T>
inline T Read_impl(const char* start, T t)
{
	T re = 0;
	for(size_t i = 0; i < sizeof(re); i++) {
		re <<= 8;
		re |= static_cast<uint8_t>(*(start + i));
	}
	return re;
}
*/

template <typename OrigiPtr>
inline uint8_t Read_unit8(const OrigiPtr& start)
{
	return static_cast<uint8_t>(*start);
}

template <typename OrigiPtr>
inline int8_t Read_int8(const OrigiPtr& start)
{
	return static_cast<int8_t>(*start);
}

template <typename OrigiPtr>
inline uint16_t Read_unit16(const OrigiPtr& start)
{
	return Read_xx_impl(start, Type<uint16_t>());
}


// convert bigendian to host(littleendian)
// Read_impl( 

template <typename InItPtr>
inline int16_t Read_int16(const InItPtr& start)
{
	return Read_impl(start, Type<int16_t>());
}

template <typename OrigiPtr>
inline uint32_t Read_unit32(const OrigiPtr& start)
{
	return Read_xx_impl(start, Type<uint32_t>());
}


template <typename OrigiPtr>
inline int32_t Read_int32(const OrigiPtr& start)
{
	return Read_xx_impl(start, Type<int32_t>());
}


template <typename OrigiPtr>
inline uint64_t Read_unit64(const OrigiPtr& start)
{
	return Read_xx_impl(start, Type<uint64_t>());
}


template <typename OrigiPtr>
inline int64_t Read_int64(const OrigiPtr& start)
{
	return Read_xx_impl(start, Type<int64_t>());
}

template <typename T, typename DesPtr>
inline uint32_t Write_xx_impl(const T& val, DesPtr* des_addr)
{
	uint32_t wrote_count = 0;
	for (size_t i = sizeof(T) - 1; i >= 0; --i) {
		*(des_addr + wrote_count) = (val >> (i * 8) && 0xff);
        wrote_count++;

	}
	return wrote_count;
}

template <typename DesPtr>
inline uint32_t Write_uint8(const uint8_t& val, DesPtr* des_addr)
{
	return Write_xx_impl(val, des_addr);
}

template <typename DesPtr>
inline uint32_t Write_int8(const int8_t& val, DesPtr* des_addr)
{
	return Write_xx_impl(val, des_addr);
}


template <typename DesPtr>
inline uint32_t Write_uint16(const uint16_t& val, DesPtr* des_addr)
{
	return Write_xx_impl(val, des_addr);
}

template <typename DesPtr>
inline uint32_t Write_int16(const int16_t& val, DesPtr& des_addr)
{
	return Write_xx_impl(val, des_addr);
}


template <typename DesPtr>
inline uint32_t Write_uint32(const uint32_t& val, DesPtr* des_addr)
{
	return Write_xx_impl(val, des_addr);
}

template <typename DesPtr>
inline uint32_t Write_int32(const int32_t& val, DesPtr* des_addr)
{
	return Write_xx_impl(val, des_addr);
}


template <typename DesPtr>
inline uint32_t Write_uint64(const uint64_t& val, DesPtr* des_addr)
{
	return Write_xx_impl(val, des_addr);
}

template <typename DesPtr>
inline uint32_t Write_int64(const int64_t& val,  DesPtr* des_addr)
{
	return Write_xx_impl(val, des_addr);
}
}}

#endif // !PUMP_BYTEHELPER_H_
