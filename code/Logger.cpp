#include "Logger.h"

//#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE // file name and line number
//#include "spdlog/spdlog.h"
//#include "spdlog/common.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"


using namespace ecron;


Logger Logger::instance_;

bool Logger::init(const char* strfile, bool async, bool truncate)
{
    if (init_ || nullptr == strfile)
        return false;
    
    try
    {
        if (async)
        {
            spdlog::init_thread_pool(8192, 1);
            logger_ = spdlog::basic_logger_st<spdlog::async_factory>("basic_logger", strfile, truncate);
            //logger_ = spdlog::basic_logger_st("basic_logger", strfile, truncate);
        }
        else
            logger_ = spdlog::basic_logger_mt("basic_logger", strfile, truncate);

        //register it if you need to access it globally
        //spdlog::register_logger(logger_);

        // set max level for wrap
        spdlog::set_level(spdlog::level::trace);

        // %e:ms
        spdlog::set_pattern("%C%m%d %T.%e %t %l %v");

        //_logger->flush_on(spdlog::level::info);
        //spdlog::flush_every(std::chrono::seconds(3));
    }
    catch (const spdlog::spdlog_ex&/* ex*/)
    {
        //std::cout << "Log init failed: " << ex.what() << std::endl;
        return false;
    }

    async_ = async;
    lv_ = LL_Info;
    init_ = true;
    return true;
}

void Logger::release()
{
    if (!init_)
        return;

    logger_->flush();
    spdlog::drop("basic_logger");
    spdlog::shutdown();

    if (async_)
        async_ = false;
    lv_ = LL_Off;
    init_ = false;
}

void Logger::flush()
{
    if (!init_)
        return;

    logger_->flush();
}

void Logger::set_level(LogLevel lv)
{
    if (!init_)
        return;

    lv_.store(lv);
}

LogLevel Logger::get_level() const
{
    if (!init_)
        return LL_Off;

    return lv_.load();
}

Logger::Logger()
    : init_(false)
    , lv_(LL_Info)
    , async_(false)
{
    //lv_.store(LL_Info);
}

Logger::~Logger()
{
    release();
}

//Logger::Logger() : impl_(new LoggerImpl())
//{
//    impl_ = std::make_unique<LoggerImpl>();
//}

//Logger::~Logger()
//{
//    release();
//}


//void test_spdlog()
//{
//    try
//    {
//        spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name) function");
//
//        // Console logger with color
//        auto console = spdlog::stdout_color_mt("console");
//        console->info("Welcome to spdlog!");
//        console->error("Some error message with arg{}..", 1);
//
//        // Conditional logging example
//        auto i = 2;
//        console->warn_if(i != 0, "an important message");
//
//        // Formatting examples
//        console->warn("Easy padding in numbers like {:08d}", 12);
//        console->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
//        console->info("Support for floats {:03.2f}", 1.23456);
//        console->info("Positional args are {1} {0}..", "too", "supported");
//        console->info("{:<30}", "left aligned");
//
//
//        spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name) function");
//
//        // Create basic file logger (not rotated)
//        auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
//        my_logger->info("Some log message");
//
//        // Create a file rotating logger with 5mb size max and 3 rotated files
//        auto rotating_logger = spdlog::rotating_logger_mt("some_logger_name", "logs/mylogfile", 1048576 * 5, 3);
//        for (int i = 0; i < 10; ++i)
//            rotating_logger->info("{} * {} equals {:>10}", i, i, i*i);
//
//        // Create a daily logger - a new file is created every day on 2:30am
//        auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily", 2, 30);
//        // trigger flush if the log severity is error or higher
//        daily_logger->flush_on(spdlog::level::err);
//        daily_logger->info(123.44);
//
//        // Customize msg format for all messages
//        spdlog::set_pattern(" **** %C-%m-%d %T %l %v ****");
//        //spdlog::set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");
//        rotating_logger->info("This is another message with custom format");
//
//
//        // Runtime log levels
//        spdlog::set_level(spdlog::level::info); //Set global log level to info
//        console->debug("This message shold not be displayed!");
//        console->set_level(spdlog::level::debug); // Set specific logger's log level
//        console->debug("This message shold be displayed..");
//
//        // Compile time log levels
//        // define SPDLOG_DEBUG_ON or SPDLOG_TRACE_ON
//        SPDLOG_TRACE(console, "Enabled only #ifdef SPDLOG_TRACE_ON..{} ,{}", 1, 3.23);
//        SPDLOG_DEBUG(console, "Enabled only #ifdef SPDLOG_DEBUG_ON.. {} ,{}", 1, 3.23);
//
//        // Asynchronous logging is very fast..
//        // Just call spdlog::set_async_mode(q_size) and all created loggers from now on will be asynchronous..
//        async_example();
//
//        // syslog example. linux/osx only
//        //syslog_example();
//
//        // android example. compile with NDK
//        //android_example();
//
//        // Log user-defined types example
//        //user_defined_example();
//
//        // Change default log error handler
//        //err_handler_example();
//
//        // Apply a function on all registered loggers
//        spdlog::apply_all([&](std::shared_ptr<spdlog::logger> l)
//        {
//            l->info("End of example.");
//        });
//
//        // Release and close all loggers
//        spdlog::drop_all();
//    }
//    // Exceptions will only be thrown upon failed logger or sink construction (not during logging)
//    catch (const spdlog::spdlog_ex& ex)
//    {
//        std::cout << "Log init failed: " << ex.what() << std::endl;
//        return;
//    }
//}
