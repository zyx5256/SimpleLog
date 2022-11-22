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

struct LogWriter {
  ~LogWriter() = default;

  template<typename T>
  LogWriter& operator<<(const T& content) noexcept
  {
    sstream_ << content;
    return *this;
  }

  std::stringstream sstream_;
};

class Logger {
public:
  virtual ~Logger() = default;

  // any operator that has lower precedence than << is fine. here we uses &
  virtual void operator&(const LogWriter& logWriter) noexcept = 0;
};

std::unique_ptr<Logger> createLogger(const char* file, int line, const char* function, const LogLevel& level,
                                     const std::__thread_id& tid);

#define LOGGER(level, threadId) *createLogger(__FILE__, __LINE__, __FUNCTION__, level, threadId) & LogWriter()
#define LOG_INFO LOGGER(LogLevel::INFO, std::this_thread::get_id())
#define LOG_WARNING LOGGER(LogLevel::WARNING, std::this_thread::get_id())
#define LOG_ERROR LOGGER(LogLevel::ERROR, std::this_thread::get_id())


#endif //SIMPLE_LOGGER_LOGGER_H
