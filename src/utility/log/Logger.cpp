#include <pump/utility/log/Logger.h>

#include <functional>
#include <string>
#include <vector>

#include <pump/Timestamp.h>
#include <pump/utility/thread/ThreadOption.h>

namespace pump { namespace utility
{
static std::vector<std::string> LogLevelMap = {"T", "D", "I", "W", "E", "F"};

LogLine::OutputFunction LogLine::g_output_function_ = [](const char* message, size_t len)
{
	size_t wrote_count = fwrite(message, 1, len, stdout);
};

LogLine::FlushFunction LogLine::g_flush_function_ = []() { fflush(stdout); };


LogLevel LogLine::g_level_ = LogLevel::PUMP_DEBUG;


LogLine::LogLine(SourceFile source_file, int line, LogLevel level)
	: base_name_(source_file),
	line_(line),
	level_(level)
{
	auto a = Timestamp::now();

	log_stream_ << '[' << Timestamp::now().get_time_point_string() << ']'
				<< '[' << LogLevelMap[level] << ']'
				<< '[' << GetCurrentThreadId() << ']';
}

LogLine::~LogLine()
{
	finish();
	g_output_function_(stream().buffer().data(), stream().buffer().length());
	g_flush_function_();
}

void LogLine::finish()
{
	log_stream_ << '[' << base_name_ << ":" << line_ << "]\n";
}
}}
