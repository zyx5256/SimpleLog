#ifndef SIMPLE_LOGGER_LOGGER_H
#define SIMPLE_LOGGER_LOGGER_H


#include <string>
#include <memory>
#include <thread>
#include <sstream>
#include "LogStream.h"

enum LogLevel {
  INFO,
  WARNING,
  ERROR,
};

class Logger {
public:
  virtual ~Logger() = default;

  virtual LogStream& buffer() = 0;

  static void setOutputFunc(const std::function<void(char*, size_t)>& outputFunc);

  static void setLevel(const LogLevel& lvl);

  static LogLevel level_;
};

std::unique_ptr<Logger> createLogger(const char* file, int line, const char* function, const std::string& level,
                                     const std::thread::id& tid);

#define LOGGER(level, threadId) createLogger(__FILE__, __LINE__, __FUNCTION__, level, threadId)->buffer()
#define LOG_INFO if (Logger::level_ <= LogLevel::INFO) LOGGER("INFO ", std::this_thread::get_id())
#define LOG_WARNING if (Logger::level_ <= LogLevel::WARNING) LOGGER("WARN ", std::this_thread::get_id())
#define LOG_ERROR if (Logger::level_ <= LogLevel::ERROR) LOGGER("ERROR", std::this_thread::get_id())


#endif //SIMPLE_LOGGER_LOGGER_H
