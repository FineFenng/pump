#ifndef PUMP_UTILITY_LOGGER_H_
#define PUMP_UTILITY_LOGGER_H_

#include <functional>

#include <pump/utility/log/LogStream.h>
#include <pump/utility/SourceFile.h>


namespace pump { namespace utility
{

enum  LogLevel
{
    PUMP_TRACE = 0,
    PUMP_DEBUG = 1, 
    PUMP_INFO  = 2,
    PUMP_WARN  = 3,
    PUMP_ERROR = 4,
    PUMP_FATAL = 5,
};


class LogLine
{
public:
    typedef std::function<void(const char*, size_t)> OutputFunction;
    typedef std::function<void()> FlushFunction;

public:
    LogLine(SourceFile source_file, int line, LogLevel level);

    ~LogLine();

    LogStream& stream() { return log_stream_; }

    LogLevel get_local_log_level() const { return level_; }

    static void set_output_function(const OutputFunction& output_function) { g_output_function_ = output_function; }
    static void set_flush_function(const FlushFunction& flush_function) { g_flush_function_ = flush_function; }
    static LogLevel get_global_log_level() { return g_level_; }
    static void set_global_log_level(const LogLevel& level) { g_level_ = level; }

private:
    void finish();

private:
    static OutputFunction g_output_function_;
    static FlushFunction  g_flush_function_;
    static LogLevel       g_level_;

private:
    SourceFile base_name_;
    LogStream log_stream_;
    int line_;
    LogLevel  level_;



};
}
}

#define LOG_TRACE if (pump::utility::LogLine::get_global_log_level() <= pump::utility::LogLevel::PUMP_TRACE) \
    pump::utility::LogLine(__FILE__, __LINE__, pump::utility::LogLevel::PUMP_TRACE).stream()

#define LOG_DEBUG if (pump::utility::LogLine::get_global_log_level() <= pump::utility::LogLevel::PUMP_DEBUG) \
    pump::utility::LogLine(__FILE__, __LINE__, pump::utility::LogLevel::PUMP_DEBUG).stream()
	
#define LOG_INFO if (pump::utility::LogLine::get_global_log_level() <= pump::utility::LogLevel::PUMP_INFO) \
    pump::utility::LogLine(__FILE__, __LINE__, pump::utility::LogLevel::PUMP_INFO).stream()
	
#define LOG_WARN if (pump::utility::LogLine::get_global_log_level() <= pump::utility::LogLevel::PUMP_WARN) \
    pump::utility::LogLine(__FILE__, __LINE__, pump::utility::LogLevel::PUMP_WARN).stream()
	
#define LOG_ERROR if (pump::utility::LogLine::get_global_log_level() <= pump::utility::LogLevel::PUMP_ERROR) \
    pump::utility::LogLine(__FILE__, __LINE__, pump::utility::LogLevel::PUMP_ERROR).stream()
	
#define LOG_FALAL if (pump::utility::LogLine::get_global_log_level() <= pump::utility::LogLevel::PUMP_FATAL) \
    pump::utility::LogLine(__FILE__, __LINE__, pump::utility::LogLevel::PUMP_FATAL).stream()



#endif //PUMP_UTILIYT_H_
