#include <pump/utility/Base64.h>
namespace  pump
{
namespace utility
{


#define REMAIN_ZERO_BYTE	0
#define REMAIN_ONE_BYTE		1
#define REMAIN_TWO_BYTE		2


namespace
{
	const BYTE_T kStandardBase64CodingString[] = 
		"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

}
void Base64::coding(const BYTE_T* in_buffer, size_t in_len, BYTE_T* out_buffer, size_t* out_len)
{
	memset(out_buffer, 0, 4);
	memcpy(out_buffer, in_buffer, in_len);
	switch (in_len) {
	case REMAIN_ZERO_BYTE: {
			out_buffer[0] = kStandardBase64CodingString[out_buffer[0] >> 2];
			out_buffer[1] = kStandardBase64CodingString[out_buffer[1] >> 2];
			out_buffer[2] = kStandardBase64CodingString[out_buffer[2] >> 2];
			out_buffer[3] = kStandardBase64CodingString[out_buffer[3] >> 2];
			break;
		}
		case REMAIN_ONE_BYTE: {
			out_buffer[0] = kStandardBase64CodingString[out_buffer[0] >> 2];
			out_buffer[1] = kStandardBase64CodingString[out_buffer[1] >> 2];
			out_buffer[2] = '=';
			out_buffer[3] = '=';
			break;
		}
		case REMAIN_TWO_BYTE: {
			out_buffer[0] = kStandardBase64CodingString[out_buffer[0] >> 2];
			out_buffer[1] = kStandardBase64CodingString[out_buffer[1] >> 2];
			out_buffer[2] = kStandardBase64CodingString[out_buffer[2] >> 2];
			out_buffer[3] = '=';
			break;
		}
		default: {
			PUMP_ASSERT(false);
		}
			
	}
}

}
}