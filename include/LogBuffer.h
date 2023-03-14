#ifndef SIMPLELOG_LOGBUFFER_H
#define SIMPLELOG_LOGBUFFER_H

#include <string>
#include <sstream>
#include <functional>

class LogBuffer {
public:
  ~LogBuffer();

  static void setOutputFunc(std::function<void(char*, size_t)> outputFunc);

  template<typename T>
  LogBuffer& operator<<(T content)
  {
    ss_ << content;
    return *this;
  }

private:
  std::stringstream ss_;
  static std::function<void(char*, size_t)> outputFunc_;
};



#endif //SIMPLELOG_LOGBUFFER_H
