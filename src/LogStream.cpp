#include "include/LogStream.h"
#include "include/AsyncLogWriter.h"


std::function<void(char*, size_t)> LogStream::outputFunc_ = nullptr;

LogStream::~LogStream()
{
  ss_ << "\n";
  if (outputFunc_) {
    outputFunc_(&ss_.str()[0], ss_.str().size());
  } else {
    printf("%s", &ss_.str()[0]);
  }
}

void LogStream::setOutputFunc(const std::function<void(char*, size_t)>& outputFunc)
{
  outputFunc_ = outputFunc;
}
