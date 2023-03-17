#include "include/AsyncLogWriter.h"
#include "include/CountDownLatch.h"
#include "include/Buffer.h"
#include <thread>
#include <atomic>
#include <condition_variable>
#include <vector>

// TODO: using 4 buffers and 2 buffer lists
class AsyncLogWriter3Impl : public AsyncLogWriter {
public:
  AsyncLogWriter3Impl();

  ~AsyncLogWriter3Impl();

  void append(char* content, size_t len) override;

  void start() override;

  void stop() override;

private:
  void threadFunc();
};

AsyncLogWriter3Impl::AsyncLogWriter3Impl()
{

}

AsyncLogWriter3Impl::~AsyncLogWriter3Impl()
{

}

void AsyncLogWriter3Impl::append(char* content, size_t len)
{

}

void AsyncLogWriter3Impl::start()
{

}

void AsyncLogWriter3Impl::stop()
{

}

void AsyncLogWriter3Impl::threadFunc()
{

}

AsyncLogWriterPtr createAsyncLogWriter3()
{
  static AsyncLogWriterPtr instance = std::make_shared<AsyncLogWriter3Impl>();
  return instance;
}
