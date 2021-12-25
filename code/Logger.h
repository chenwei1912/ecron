#ifndef ECRON_LOGGER_H
#define ECRON_LOGGER_H

#include "spdlog/fmt/fmt.h"
#include "Buffer.h"


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
//        try {
        BufferPtr buffer = std::make_shared<Buffer>();
        //if (!buffer) exit();
        // message len not more than buffer len
        fmt::format_to(buffer->begin_write(), format_str, args...);
        //std::string msg = fmt::format(format_str, args...);
        //buffer.has_written(it.count()); // auto it = fmt::format_to
        log_buffer(lv, buffer);
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
	void log_string(LogLevel lv, const std::string& str);
	void log_buffer(LogLevel lv, const BufferPtr& buffer);

	class LoggerImpl;
	std::unique_ptr<LoggerImpl> impl_;
};


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
