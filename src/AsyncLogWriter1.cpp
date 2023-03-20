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
  std::mutex writeMtx_;
  std::condition_variable waitOnCurBuf_;
  std::condition_variable waitOnWriteBuf_;
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
  waitOnWriteBuf_.notify_all();
  if (td_.joinable()) {
    td_.join();
    printf("AsyncLogWriter1: end.\n");
  }
}

void AsyncLogWriter1Impl::append(char* content, size_t len)
{
  std::unique_lock<std::mutex> ul(pushMtx_);
  while (curBuffer_->avail() <= len) {
    waitOnCurBuf_.wait(ul, [&]{return curBuffer_->avail() > len;});
  }
  curBuffer_->append(content, len);

  if (curBuffer_->size() > 2 * curBuffer_->avail()) {
    printf("%c append: swap\n", content[len - 2]); // DEBUG
    std::lock_guard<std::mutex> guard(writeMtx_);
    std::swap(curBuffer_, bufferToWrite_);
    curBuffer_->resetPos();
    waitOnCurBuf_.notify_all();
    waitOnWriteBuf_.notify_all();
  }
}

void AsyncLogWriter1Impl::threadFunc()
{
  latch_->wait();
  while (running_) {
    std::unique_lock<std::mutex> ul(writeMtx_);
    while (running_ && bufferToWrite_->empty()) {
      waitOnWriteBuf_.wait(ul, [&]{return !bufferToWrite_->empty() || !running_;});
    }
    printf("%.*s", static_cast<int>(bufferToWrite_->size()), bufferToWrite_->data());
    bufferToWrite_->resetPos();
  }

  printf("write: rest of the log lines\n"); // DEBUG
  std::unique_lock<std::mutex> ul(pushMtx_);
  printf("%.*s", static_cast<int>(curBuffer_->size()), curBuffer_->data()); // write rest of the log lines in current buffer
}

AsyncLogWriterPtr createAsyncLogWriter1()
{
  static AsyncLogWriterPtr instance = std::make_shared<AsyncLogWriter1Impl>();
  return instance;
}