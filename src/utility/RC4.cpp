#include <pump/utility/RC4.h>

#include <cstring>
#include <utility>

#include <pump/Common.h>



namespace pump
{
namespace utility
{
	RC4::RC4(const char* key)
		:box_{0}
	{
	}

	bool RC4::set_key(const char* key)
	{
		if (key == nullptr) {
			return false;
		}


		const size_t key_len = strlen(key);
		init_box();
		mix_box(key, PUMP_MIN(key_len, kMaxKeySize));
		return true;
	}

	void RC4::encrypt(void* buffer, size_t len) const
	{
		BYTE_T temp_box[kMaxKeySize];
		memcpy(temp_box, box_, kMaxKeySize);

		size_t i = 0;
		size_t j = 0;
		auto* data = reinterpret_cast<BYTE_T*>(buffer);

		for (size_t k = 0; k < len; ++k) {
			i = (i + 1) % kMaxKeySize;
			j = (j + 1) % kMaxKeySize;
			std::swap(temp_box[i], temp_box[j]);

			const size_t pos = (temp_box[i] + temp_box[j]) % kMaxKeySize;
			*data ^= temp_box[pos];
			++data;
		}
	}

	void RC4::decrypt(void* buffer, size_t len) const
	{
		encrypt(buffer, len);
	}

	void RC4::init_box()
	{
		for(size_t i = 0; i < kMaxKeySize; ++i) {
			box_[i] = i;
		}
	}

	void RC4::mix_box(const char* real_key, size_t key_len)
	{
		int calc_index = 0;
		for (size_t index = 0; index < kMaxKeySize; ++index) {
			calc_index = (calc_index + box_[index] + real_key[index % key_len]) % kMaxKeySize;
			std::swap(box_[index], box_[calc_index]);
		}
	}
}
}