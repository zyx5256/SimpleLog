#include "include/Logger.h"
#include <unordered_map>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

// Logger: static methods and variables
LogLevel initLogLevel() {
  std::unordered_map<std::string, LogLevel> NAME_LOG_MAP{
    {"INFO",    LogLevel::INFO},
    {"WARNING", LogLevel::WARNING},
    {"ERROR",   LogLevel::ERROR},
  };

  char* levelStr = std::getenv("LOG_LEVEL");

  if (levelStr == nullptr || NAME_LOG_MAP.find(levelStr) == NAME_LOG_MAP.end()) {
    printf("LOG LEVEL %s is not defined. Using default level: INFO\n", levelStr);
    return LogLevel::INFO;
  } else {
    printf("LOG LEVEL set: %s\n", levelStr);
    return NAME_LOG_MAP[levelStr];
  }
}

LogLevel Logger::level_ = initLogLevel();

void Logger::setLevel(const LogLevel& lvl)
{
  Logger::level_ = lvl;
}

// Logger implementation
class LoggerImpl : public Logger {
public:
  LoggerImpl(const char* fileName, int lineNo, const char* funcName, const std::string& level, std::thread::id tid);

  LogBuffer& buffer() override;

private:
  void formatTime();

private:
  char time_[80]{};
  LogBuffer logBuffer_;
};


LoggerImpl::LoggerImpl(const char* fileName, int lineNo, const char* funcName, const std::string& level,
                       std::thread::id tid)
{

  formatTime();
  logBuffer_ << level << " " << time_ << " " << tid << " " << fileName << " " << funcName << " " << lineNo << " ";
}

void LoggerImpl::formatTime()
{
  struct timeval cur_time{};
  gettimeofday(&cur_time, nullptr);
  struct tm now{};
  localtime_r(&cur_time.tv_sec, &now);
  strftime(time_, 80, "%Y-%m-%d-%H:%M:%S", &now);
}

LogBuffer& LoggerImpl::buffer()
{
  return logBuffer_;
}

std::unique_ptr<Logger> createLogger(const char* file, int line, const char* function, const std::string& level,
                                     const std::thread::id& tid)
{
  return std::make_unique<LoggerImpl>(file, line, function, level, tid);
}