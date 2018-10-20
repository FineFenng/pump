#include <pump/utility/log/LogLine.h>

#include <vector>

#include <pump/Timestamp.hpp>
#include <pump/utility/thread/ThreadOption.h>
#include <pump/utility/log/LoggerManager.h>

namespace pump { namespace utility {
static std::vector<std::string> LogLevelMap = {"T", "D", "I", "W", "E", "F"};

LogLine::OutputFunction LogLine::g_output_function_ = [](const char* message, size_t len) {
  size_t wrote_count = fwrite(message, 1, len, stdout);
  (void) wrote_count;
};

LogLine::FlushFunction LogLine::g_flush_function_ = []() { fflush(stdout); };

LogLevel LogLine::g_level_ = LogLevel::PUMP_DEBUG;

LogLine::LogLine(SourceFile source_file, int line, LogLevel level)
	: base_name_(source_file),
	  line_(line),
	  level_(level) {
  char thread_id_str[20] = {0};
  snprintf(thread_id_str, sizeof(thread_id_str), "0x%llX", GetCurrentThreadId());

  log_stream_ << "[T-" << Timestamp::now().get_time_point_string() << ']'
			  << "[L-" << LogLevelMap[level] << ']'
			  << "[I-" << thread_id_str << ']';
}

LogLine::~LogLine() {
  finish();
  LogManager::get_instance()->write_into_logger(reinterpret_cast<const BYTE_T*>(log_stream_.buffer().data()),
												log_stream_.buffer().length());
}

void LogLine::finish() {
  log_stream_ << '[' << base_name_ << ":" << line_ << "]\n";
}
}}
