//
// Created by fenng on 18-8-3.
//


#include <pump/net/Buffer.h>

namespace pump { namespace net
{
static constexpr unsigned int kDefaultBufferSize = 64 * 1024; // 64Kbytes


int Buffer::recv_from_fd(SOCKET fd, int* saved_errno)
{
	char extra_buffer[kDefaultBufferSize] = {0};
	size_t count = 0;

#if defined(PUMP_PLATFORM_GNU) || defined(PUMP_PLATFORM_MACX)

			struct iovec iov[2];
			iov[0].iov_base = get_writable_address();
			iov[0].iov_len = get_writable_bytes();
			iov[1].iov_base = extra_buffer;
			iov[1].iov_len = kDefaultBufferSize;

			count = ::readv(fd, iov, 2);
			if (count < 0) {
				*saved_errno = errno;
			}
			else if (count <= get_writable_bytes()) {
				writable_index_ += count;
			}
			else if (count > get_writable_bytes()) {
				append_string(extra_buffer, count - get_writable_bytes());
			}
			else {

			}
#endif

#ifdef PUMP_PLATFORM_WIN
	count = Recv(fd, extra_buffer, kDefaultBufferSize, 0, saved_errno);

	if (count > 0 && count <= get_writable_bytes()) {
		append_string(extra_buffer, count);
	}
	else {
		return 0;
	}

#endif
	return count;
}
}}
