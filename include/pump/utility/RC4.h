#ifndef PUMP_UTILITY_RC4_H
#define PUMP_UTILITY_RC4_H

#include <pump/Common.h>

namespace pump
{
namespace utility
{

class RC4
{
public:
	static const int kMaxKeySize = 256;

public:
	explicit RC4(const char* key);
	RC4() = default;
	~RC4() = default;

	bool set_key(const char* key);

	void encrypt(void* buffer, size_t len) const;
	void decrypt(void* buffer, size_t len) const;
	
private:
	void init_box();
	void mix_box(const char* real_key, size_t key_len);

private:
	BYTE_T box_[kMaxKeySize];



	
			
};

} }
#endif
