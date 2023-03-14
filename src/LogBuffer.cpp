#include "include/LogBuffer.h"
#include "include/AsyncLogWriter.h"

extern AsyncLogWriterPtr GWRITER;

std::function<void(char*, size_t)> LogBuffer::outputFunc_ = nullptr;

LogBuffer::~LogBuffer()
{
  ss_ << "\n";
  if (outputFunc_ == nullptr) {
    GWRITER->append(&ss_.str()[0], ss_.str().size());
  } else {
    outputFunc_(&ss_.str()[0], ss_.str().size());
  }
}

void LogBuffer::setOutputFunc(std::function<void(char*, size_t)> outputFunc)
{
  outputFunc_ = outputFunc;
}
