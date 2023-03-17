#include "include/AsyncLogWriter.h"
#include "include/CountDownLatch.h"
#include "include/Buffer.h"
#include <thread>
#include <atomic>
#include <condition_variable>

// TODO: using only 2 buffers
class AsyncLogWriter1Impl : public AsyncLogWriter {
public:
  AsyncLogWriter1Impl();

  ~AsyncLogWriter1Impl();

  void append(char* content, size_t len) override;

  void start() override;

  void stop() override;

private:
  void threadFunc();

private:
  std::unique_ptr<LargeBuffer> curBuffer_;
  std::unique_ptr<LargeBuffer> bufferToWrite_;
  std::atomic<bool> running_;
  std::mutex pushMtx_;
  std::shared_ptr<CountDownLatch> latch_;
  std::thread td_;
};

AsyncLogWriter1Impl::AsyncLogWriter1Impl() :
  running_(false),
  latch_(createCountDownLatch(1)),
  td_(&AsyncLogWriter1Impl::threadFunc, this)
{
  curBuffer_ = std::make_unique<LargeBuffer>();
  bufferToWrite_ = std::make_unique<LargeBuffer>();
  curBuffer_->setZero();
  bufferToWrite_->setZero();
}

AsyncLogWriter1Impl::~AsyncLogWriter1Impl()
{
  AsyncLogWriter1Impl::stop();
}

void AsyncLogWriter1Impl::start()
{
  printf("AsyncLogWriter1: start.\n");
  running_ = true;
  latch_->countDown();
}

void AsyncLogWriter1Impl::stop()
{
  latch_->countDownToZero();
  running_ = false;
  if (td_.joinable()) {
    td_.join();
    printf("AsyncLogWriter1: end.\n");
  }
}

void AsyncLogWriter1Impl::append(char* content, size_t len)
{
  std::lock_guard<std::mutex> guard(pushMtx_);
  size_t availLen = curBuffer_->avail();
  if (availLen >= len) {
    curBuffer_->append(content, len);
    return;
  }

  // TODO
}

void AsyncLogWriter1Impl::threadFunc()
{
  latch_->wait();
  while (running_) {
    // TODO
  }
}

AsyncLogWriterPtr createAsyncLogWriter1()
{
  static AsyncLogWriterPtr instance = std::make_shared<AsyncLogWriter1Impl>();
  return instance;
}