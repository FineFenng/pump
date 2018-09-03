#include <pump/Basic.h>

#include <cstdarg>
#include <cstdio>
#include <exception>
#include <cstring>

namespace pump
{
#define _ASSERT_INFO_MAX_LEN 512
#define _ASSERT_MSG_MAX_LEN 512
#define _ASSERT_MSG_TOTAL_LEN (_ASSERT_INFO_MAX_LEN + _ASSERT_MSG_MAX_LEN)

void assert_failed(const char* check, const char* file, int line, const char* function, ...)
{
	char _info[_ASSERT_INFO_MAX_LEN] = {0};
	va_list args;
	va_start(args, function);
	char* fmt = va_arg(args, char*);
	const int count_1 = vsnprintf(_info, _ASSERT_INFO_MAX_LEN, fmt, args);
	va_end(args);

	if (count_1 < 0) {
		throw std::exception("assert: call vsnprintf failed.");
	}

	if (count_1 == 0) {
		const char* _no_info_attached = "no info";
		strncpy(_info, _no_info_attached, strlen(_no_info_attached));
	}

	char _message[_ASSERT_MSG_TOTAL_LEN] = {0};
	const int count_2 = snprintf(_message + count_1, _ASSERT_MSG_TOTAL_LEN,
								"assert: %s, info: %s\nfile: %s\nline : %d\nfunc : %s",
								check, _info, file, line, function);
	if (count_2 < 0) {
		throw std::exception("assert: format failed.");
	}

	throw std::exception(_message);
}
}
