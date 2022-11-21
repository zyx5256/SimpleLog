#include "Logger.h"
#include <unordered_map>
#include <sstream>
#include <chrono>
#include <ctime>
#include <sys/time.h>

extern LogLevel GLOG_LEVEL;

namespace {
std::unordered_map<LogLevel, std::string> LOG_NAME_MAP {
    {LogLevel::INFO,    "INFO"},
    {LogLevel::WARNING, "WARNING"},
    {LogLevel::ERROR,   "ERROR"},
};
}

//class LoggerImpl : public Logger {
//public:
//};

Logger::Logger(const char* fileName, int lineNo, const char* funcName, LogLevel level, std::__thread_id tid) :
               fileName_(fileName), lineNo_(lineNo), funcName_(funcName), level_(level), tid_(tid)
{
    struct timeval cur_time {};
    gettimeofday(&cur_time, nullptr);
    struct tm now;
    localtime_r(&cur_time.tv_sec, &now);
    strftime(time_, 80, "%Y-%m-%d-%H:%M:%S", &now);
    sstream_ = std::make_shared<std::stringstream>();
}

void Logger::log(const std::string& content)
{
    if (level_ < GLOG_LEVEL) {
        return;
    }
    std::stringstream ss;
    ss << "[" << LOG_NAME_MAP[level_] << "] "
       << "(" << time_
       << ", " << tid_
       << ")[" << fileName_
       << ":" << funcName_
       << ":" << lineNo_
       << "] " << content;
    printf("%s\n", &ss.str()[0]);
}
//
//template<typename T>
//Logger& Logger::operator<<(const T& content) noexcept
//{
//    (*sstream_) << content;
//    return *this;
//}
