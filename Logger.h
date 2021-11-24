#ifndef NETLIB_LOGGER_H
#define NETLIB_LOGGER_H

#include "spdlog/fmt/fmt.h"

namespace netlib
{

enum LogLevel
{
    LL_Trace = 0,
    LL_Debug,
    LL_Info,
    LL_Warn,
    LL_Error,
    LL_Critical,
    LL_Off
};

#define LOGGER Logger::instance_


class Logger
{
public:
    static Logger instance_;

    bool init(const char* file, bool truncate = false);
    void release();


    template<typename... Tn>
    void write_log(LogLevel lv, const char* fmt, const Tn&... tn)
    {
        if (filter_level(lv) || nullptr == fmt)
            return;

        std::string msg = fmt::format(fmt, tn...);
        log_string(lv, msg);
    }

    void flush();

    void set_level(LogLevel lv);
    LogLevel get_level() const;

private:
	Logger();
	~Logger();

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

    bool filter_level(LogLevel lv) const;
	void log_string(LogLevel lv, std::string& str);

	class LoggerImpl;
	std::unique_ptr<LoggerImpl> impl_;
};

}// namespace netlib

#endif // NETLIB_LOGGER_H