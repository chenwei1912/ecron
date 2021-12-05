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

    bool init(const char* file, bool async = false, bool truncate = false);
    void release();

//    void myprint()
//    {
//        std::cout << std::endl;
//    }

//    template <typename T, typename... Args>
//    void myprint(T head, Args... rest)
//    {
//       std::cout << head << ", ";
//       myprint(rest...);
//    }

    template<typename... Args>
    void write_log(LogLevel lv, fmt::string_view format_str, Args&&... args)
    {
        if (filter_level(lv))
            return;

//        try {
        std::string msg = fmt::format(format_str, args...);
        log_string(lv, msg);
//        } catch (const fmt::v5::format_error& ex) {
//            std::cout << "format_error - " << sizeof...(args) << " - "<< format_str.data() << std::endl;
//            myprint(args...);
//            return;
//        }
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