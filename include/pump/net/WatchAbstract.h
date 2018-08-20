//
// Created by finefenng on 2018/7/5.
//

#ifndef QPSTEST_HANDLE_H
#define QPSTEST_HANDLE_H

#include <cstdlib>
#include <functional>

#include <pump/Common.h>


typedef unsigned int HandleEvent;

namespace pump
{
	namespace net
	{
        enum  IO_Flag: unsigned int
        {
            kIOReadable = 0x01 ,
            kIOWritable = 0x02,
            kIReadableAndWritalbe = 0x04,
        };


		class WatchAbstract
		{
		public:

			typedef std::function<void()> ReadableCallback;
			typedef std::function<void()> WritableCallback;
			typedef std::function<void()> ErroneousCallback;

			virtual void handle_callback(const HandleEvent& handle_event) const  = 0;

			virtual int get_index() const = 0;

			virtual void set_index(int index) const = 0;

			virtual SOCKET get_fd() const = 0;

			virtual unsigned int get_events() const = 0;

			virtual void set_events(unsigned int) = 0;

			virtual bool is_readable() const = 0;

			virtual void enable_readable() = 0;

			virtual void disable_readable() = 0;

			virtual bool is_writable() const = 0;

			virtual void enable_writable() = 0;

			virtual void disable_writable() = 0;

			virtual void disable_all() = 0;

			virtual void set_readable_callback(const ReadableCallback& cb) = 0;

			virtual void set_writable_callback(const WritableCallback& cb) = 0;

			virtual void set_erroneous_callback(const ErroneousCallback& cb) = 0;

		};

	}
}

#endif //QPSTEST_HANDLE_H
