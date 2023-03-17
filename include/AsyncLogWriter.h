#ifndef SIMPLELOG_ASYNCLOGWRITER_H
#define SIMPLELOG_ASYNCLOGWRITER_H


#include <cstring>
#include <memory>
#include <mutex>

class AsyncLogWriter {
public:
  ~AsyncLogWriter() = default;

  virtual void start() = 0;

  virtual void stop() = 0;

  virtual void append(char* content, size_t len) = 0;
};

typedef std::shared_ptr<AsyncLogWriter> AsyncLogWriterPtr;

AsyncLogWriterPtr createAsyncLogWriter1();

AsyncLogWriterPtr createAsyncLogWriter2();

AsyncLogWriterPtr createAsyncLogWriter3();

#endif //SIMPLELOG_ASYNCLOGWRITER_H
