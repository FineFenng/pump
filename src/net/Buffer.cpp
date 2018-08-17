//
// Created by fenng on 18-8-3.
//


#include <pump/net/Buffer.h>
#include <pump/Common.h>


namespace pump {namespace net
{
#define  EXTRA_SIZE  65536

int Buffer::append_from_fd(int fd)
{
	char extra_buffer[EXTRA_SIZE] = {0};
	size_t count = 0;

#ifdef PUMP_PLATFORM_GNU || PUMP_PLATFORM_MACX

	struct iovec iov[2];
	iov[0].iov_base = get_writable_address();
	iov[0].iov_len = get_writable_bytes();
	iov[1].iov_base = extra_buffer;
	iov[1].iov_len = 65536;

	count = ::readv(fd, iov, 2);
	if (count < 0) {
		/*TODO*/
	}
	else if (count <= get_writable_bytes()) {
		writable_index_ += count;
	}
	else if (count > get_writable_bytes()) {
		append_string(extra_buffer, count - get_writable_bytes());
	}
	else {
		/*count == 0*/

	}
#endif

#ifdef PUMP_PLATFORM_WIN
	count = ::recv(fd, extra_buffer, EXTRA_SIZE, 0);

	if (count < 0) {
		/*TODO*/
	}
	else if (count <= get_writable_bytes()) {
		append_string(extra_buffer, count);
	}
	else { /*count == 0*/


	}
#endif
	return count;
}
}}
