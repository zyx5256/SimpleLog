#include "Logger.h"
#include <unordered_map>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

namespace {
std::unordered_map<LogLevel, std::string> LOG_NAME_MAP{
  {LogLevel::INFO,    "INFO"},
  {LogLevel::WARNING, "WARNING"},
  {LogLevel::ERROR,   "ERROR"},
};

std::unordered_map<std::string, LogLevel> NAME_LOG_MAP{
  {"INFO",    LogLevel::INFO},
  {"WARNING", LogLevel::WARNING},
  {"ERROR",   LogLevel::ERROR},
};

LogLevel LevelSetter()
{
  std::string levelStr;
  try {
    levelStr = std::getenv("LOG_LEVEL");
  } catch (std::exception& e) {
    printf("get env LOG LEVEL failed.\n");
  }

  if (NAME_LOG_MAP.find(levelStr) == NAME_LOG_MAP.end()) {
    printf("LOG LEVEL:%s is not defined. Using default level: INFO\n", levelStr.c_str());
    return LogLevel::INFO;
  } else {
    printf("LOG LEVEL set: %s\n", levelStr.c_str());
    return NAME_LOG_MAP[levelStr];
  }
}
}

class LoggerImpl : public Logger {
public:
  LoggerImpl(const char* fileName, int lineNo, const char* funcName, LogLevel level, std::__thread_id tid);

  void operator&(const LogWriter& logWriter) noexcept override;

private:
  const char* fileName_;
  int lineNo_;
  const char* funcName_;
  std::__thread_id tid_;
  char time_[80]{};
  LogLevel level_;
  static LogLevel GLOG_LEVEL;
};

// set log level at compile stage
LogLevel LoggerImpl::GLOG_LEVEL = LevelSetter();

LoggerImpl::LoggerImpl(const char* fileName, int lineNo, const char* funcName, LogLevel level, std::__thread_id tid) :
  fileName_(fileName), lineNo_(lineNo), funcName_(funcName), level_(level), tid_(tid)
{
  struct timeval cur_time{};
  gettimeofday(&cur_time, nullptr);
  struct tm now{};
  localtime_r(&cur_time.tv_sec, &now);
  strftime(time_, 80, "%Y-%m-%d-%H:%M:%S", &now);
}

void LoggerImpl::operator&(const LogWriter& logWriter) noexcept
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
     << "] " << logWriter.sstream_.rdbuf();
  printf("%s\n", ss.str().c_str());
}

std::unique_ptr<Logger> createLogger(const char* file, int line, const char* function, const LogLevel& level,
                                     const std::__thread_id& tid)
{
  return std::make_unique<LoggerImpl>(LoggerImpl(file, line, function, level, tid));
}