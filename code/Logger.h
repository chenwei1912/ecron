#ifndef ECRON_LOGGER_H
#define ECRON_LOGGER_H

//#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // file name and line number
#include "spdlog/logger.h"


namespace ecron
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

    void flush();
    void set_level(LogLevel lv);
    LogLevel get_level() const;

    template<typename... Args>
    void write_log(LogLevel lv, const char* fmt, const Args&... args);

    inline bool is_init() const { return init_; }

private:
	Logger();
	~Logger();

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;

    bool init_;
    std::atomic<LogLevel> lv_;
    bool async_;
    std::shared_ptr<spdlog::logger> logger_;
	//class LoggerImpl;
	//std::unique_ptr<LoggerImpl> impl_;
};

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
void Logger::write_log(LogLevel lvl, const char* fmt, const Args&... args)
{
    spdlog::level::level_enum lv_spd = spdlog::level::off;
    switch (lvl)
    {
    case LL_Trace:
        lv_spd = spdlog::level::trace;
        break;
    case LL_Debug:
        lv_spd = spdlog::level::debug;
        break;
    case LL_Info:
        lv_spd = spdlog::level::info;
        break;
    case LL_Warn:
        lv_spd = spdlog::level::warn;
        break;
    case LL_Error:
        lv_spd = spdlog::level::err;
        break;
    case LL_Critical:
        lv_spd = spdlog::level::critical;
        break;
    //case LL_Off:
    //    lv_spd = spdlog::level::off;
    //    break;
    default:
        return;
    }

    //try {
    //std::string msg = fmt::format(fmt, args...); // auto it = fmt::format_to
    logger_->log(lv_spd, fmt, args...);
    //} catch (const fmt::v5::format_error& ex) {
    //    std::cout << "format_error - " << sizeof...(args) << " - "<< format_str.data() << std::endl;
    //    myprint(args...);
    //    return;
    //}
}


// __FILENAME__ macro is define by cmake(g++ flag)
// -D__FILENAME__='\"$(subst $(dir $<),,$<)\"'")
#define LOG_TRACE(fmt, ...)                                                           \
    do {                                                                              \
        if (ecron::LOGGER.is_init() && ecron::LOGGER.get_level() <= ecron::LL_Trace) \
            ecron::LOGGER.write_log(ecron::LL_Trace, fmt " - {}:{}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__, __func__);                             \
    } while (0)

#define LOG_DEBUG(fmt, ...)                                                           \
    do {                                                                              \
        if (ecron::LOGGER.is_init() && ecron::LOGGER.get_level() <= ecron::LL_Debug) \
            ecron::LOGGER.write_log(ecron::LL_Debug, fmt " - {}:{}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__, __func__);                             \
    } while (0)

#define LOG_INFO(fmt, ...)                                                            \
    do {                                                                              \
        if (ecron::LOGGER.is_init() && ecron::LOGGER.get_level() <= ecron::LL_Info) \
            ecron::LOGGER.write_log(ecron::LL_Info, fmt,                  \
            ##__VA_ARGS__);                             \
    } while (0)

#define LOG_WARN(fmt, ...)                                                            \
    do {                                                                              \
        if (ecron::LOGGER.is_init() && ecron::LOGGER.get_level() <= ecron::LL_Warn) \
            ecron::LOGGER.write_log(ecron::LL_Warn, fmt " - {}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__);                             \
    } while (0)

#define LOG_ERROR(fmt, ...)                                                           \
    do {                                                                              \
        if (ecron::LOGGER.is_init() && ecron::LOGGER.get_level() <= ecron::LL_Error) \
            ecron::LOGGER.write_log(ecron::LL_Error, fmt " - {}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__);                             \
    } while (0)

#define LOG_CRITICAL(fmt, ...)                                                        \
    do {                                                                              \
        if (ecron::LOGGER.is_init() && ecron::LOGGER.get_level() <= ecron::LL_Critical) \
            ecron::LOGGER.write_log(ecron::LL_Critical, fmt " - {}:{}",                  \
            ##__VA_ARGS__, __FILENAME__, __LINE__);                             \
    } while (0)


}// namespace ecron

#endif // ECRON_LOGGER_H
