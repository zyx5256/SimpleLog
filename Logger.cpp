#include "Logger.h"
#include <unordered_map>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <sys/time.h>

LogLevel GLOG_LEVEL = LogLevel::INFO;

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

struct LevelSetter {
  LevelSetter()
  {
    std::string levelStr = std::getenv("LOG_LEVEL");
    if (NAME_LOG_MAP.find(levelStr) == NAME_LOG_MAP.end()) {
      printf("LOG LEVEL %s not defined. Using default level: INFO\n", levelStr.c_str());
    } else {
      GLOG_LEVEL = NAME_LOG_MAP[levelStr];
    }
  }
};
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
  static LevelSetter levelSetter_;
};

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