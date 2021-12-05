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

    bool is_init();
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

    template<typename... Args> // fmt::string_view
    void write_log(LogLevel lv, const char* format_str, const Args&... args)
    {
//        if (filter_level(lv))
//            return;

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

    //bool filter_level(LogLevel lv) const;
	void log_string(LogLevel lv, std::string& str);

	class LoggerImpl;
	std::unique_ptr<LoggerImpl> impl_;
};


// __FILENAME__ macro is define by cmake(g++ flag)
// -D__FILENAME__='\"$(subst $(dir $<),,$<)\"'")
#define LOG_TRACE(fmt, ...)                                                           \
    do {                                                                              \
        if (netlib::LOGGER.is_init() && netlib::LOGGER.get_level() <= netlib::LL_Trace) \
            netlib::LOGGER.write_log(netlib::LL_Trace, fmt " - {}:{}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__, __func__);                             \
    } while (0)

#define LOG_DEBUG(fmt, ...)                                                           \
    do {                                                                              \
        if (netlib::LOGGER.is_init() && netlib::LOGGER.get_level() <= netlib::LL_Debug) \
            netlib::LOGGER.write_log(netlib::LL_Debug, fmt " - {}:{}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__, __func__);                             \
    } while (0)

#define LOG_INFO(fmt, ...)                                                            \
    do {                                                                              \
        if (netlib::LOGGER.is_init() && netlib::LOGGER.get_level() <= netlib::LL_Info) \
            netlib::LOGGER.write_log(netlib::LL_Info, fmt,                  \
            ##__VA_ARGS__);                             \
    } while (0)

#define LOG_WARN(fmt, ...)                                                            \
    do {                                                                              \
        if (netlib::LOGGER.is_init() && netlib::LOGGER.get_level() <= netlib::LL_Warn) \
            netlib::LOGGER.write_log(netlib::LL_Warn, fmt " - {}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__);                             \
    } while (0)

#define LOG_ERROR(fmt, ...)                                                           \
    do {                                                                              \
        if (netlib::LOGGER.is_init() && netlib::LOGGER.get_level() <= netlib::LL_Error) \
            netlib::LOGGER.write_log(netlib::LL_Error, fmt " - {}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__);                             \
    } while (0)

#define LOG_CRITICAL(fmt, ...)                                                        \
    do {                                                                              \
        if (netlib::LOGGER.is_init() && netlib::LOGGER.get_level() <= netlib::LL_Critical) \
            netlib::LOGGER.write_log(netlib::LL_Critical, fmt " - {}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__);                             \
    } while (0)

}// namespace netlib

#endif // NETLIB_LOGGER_H
