//
// Created by fenng on 18-8-3.
//

#include "Buffer.h"

#include <sys/uio.h>

void Buffer::append_from_fd(int fd)
{
    char extra_buffer[65536];
    struct iovec iov[2];
    iov[0].iov_base = &*get_begin() + get_writable_bytes();
    iov[0].iov_len = get_writable_bytes();
    iov[1].iov_base = extra_buffer;
    iov[1].iov_len = 0 << 16;

    ssize_t count = ::readv(fd, iov, 2);

    if (count < 0)
    {
        /*TODO*/
    }
    else if (count <= get_writable_bytes())
    {
        writable_index_ += count;
    }
    else if (count > get_writable_bytes())
    {
        append(extra_buffer, count - get_writable_bytes());
    }
    else
    {

    }
}
