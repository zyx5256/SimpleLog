#include "include/AsyncLogWriter.h"
#include <thread>
#include <atomic>
#include <condition_variable>

class AsyncLogWriterImpl : public AsyncLogWriter {
public:
  AsyncLogWriterImpl();

  ~AsyncLogWriterImpl();

  void append(char* content, size_t len) override;

  void start() override;

  void stop() override;

private:
  void threadFunc();

private:
  static const int kLargeBuffer = 400;
  char curBuffer_[kLargeBuffer];
  char backupBuffer_[kLargeBuffer];
  std::mutex mtx_;
  std::condition_variable cv_;
  std::thread td_;
  std::atomic<bool> running_;
  int curPos_ = 0;
};

AsyncLogWriterImpl::AsyncLogWriterImpl() :
running_(false),
mtx_(),
cv_(),
td_(&AsyncLogWriterImpl::threadFunc, this)
{
  memset(curBuffer_, 0, kLargeBuffer);
  memset(backupBuffer_, 0, kLargeBuffer);
}

AsyncLogWriterImpl::~AsyncLogWriterImpl()
{

}

void AsyncLogWriterImpl::start()
{
  printf("start.\n");
  running_ = true;
  cv_.notify_all();
}

void AsyncLogWriterImpl::stop()
{
  td_.join();
  printf("Async Logging thread finished.\n");
}

void AsyncLogWriterImpl::append(char* content, size_t len)
{
  printf("try append %c.\n", content[len - 2]);
  {
    std::lock_guard<std::mutex> guard(mtx_);
    memcpy(curBuffer_ + curPos_, content, len);
    curPos_ += len;
  }
  printf("append %c.\n", content[len - 2]);
}

void AsyncLogWriterImpl::threadFunc()
{
  std::unique_lock<std::mutex> ul(mtx_);
  printf("wait.\n");
  cv_.wait(ul, [&]() -> std::atomic<bool>& { return running_; });
  printf("lock buffer.\n");
  printf("%s\n", curBuffer_);
}

AsyncLogWriterPtr createAsyncLogWriter()
{
  static AsyncLogWriterPtr instance = std::make_shared<AsyncLogWriterImpl>();
  return instance;
}