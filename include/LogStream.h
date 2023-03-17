#ifndef SIMPLELOG_LOGSTREAM_H
#define SIMPLELOG_LOGSTREAM_H

#include <string>
#include <sstream>
#include <functional>

class LogStream {
public:
  ~LogStream();

  static void setOutputFunc(const std::function<void(char*, size_t)>& outputFunc);

  template<typename T>
  LogStream& operator<<(T content)
  {
    ss_ << content;
    return *this;
  }

private:
  std::stringstream ss_;
  static std::function<void(char*, size_t)> outputFunc_;
};



#endif //SIMPLELOG_LOGSTREAM_H
