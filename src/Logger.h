#ifndef SIMPLE_LOGGER_LOGGER_H
#define SIMPLE_LOGGER_LOGGER_H


#include <string>
#include <memory>
#include <thread>
#include <sstream>

enum LogLevel {
INFO,
WARNING,
ERROR,
};

class Logger {
public:
    Logger(const char* fileName, int lineNo, const char* funcName, LogLevel level, std::__thread_id tid);
    virtual ~Logger() = default;
    void log(const std::string& content);
    template<typename T>
    Logger& operator<<(const T& content) noexcept // TODO
    {
        (*sstream_) << content;
        log(sstream_->str());
        return *this;
    }

private:
    const char* fileName_;
    int lineNo_;
    const char* funcName_;
    std::__thread_id tid_;
    char time_[80];
    LogLevel level_;
    std::shared_ptr<std::stringstream> sstream_;
};

#define LOGGER(level, threadId) Logger(__FILE__, __LINE__, __FUNCTION__, level, threadId)
#define LOG_INFO LOGGER(LogLevel::INFO, std::this_thread::get_id())
#define LOG_WARNING LOGGER(LogLevel::WARNING, std::this_thread::get_id())
#define LOG_ERROR LOGGER(LogLevel::ERROR, std::this_thread::get_id())


#endif //SIMPLE_LOGGER_LOGGER_H
