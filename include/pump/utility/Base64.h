#ifndef PUMP_UTILITY_BASE64_H
#define PUMP_UTILITY_BASE64_H

#include <pump/Common.h>

namespace pump
{
namespace utility
{
	enum Base64CodeType
	{
		kBase64Standard,
		kBase64Url,
	};
class Base64
{ 
public:
	explicit Base64(const Base64CodeType = kBase64Standard);
	~Base64() = default;

	void Encode(const void* in_buffer, size_t in_len, void* out_buffer, size_t* out_len);


	
private:
	void coding(const BYTE_T* in_buffer, size_t in_len, BYTE_T* out_buffer, size_t* out_len);

private:
	Base64CodeType code_type_;
};


		
}}

#endif
