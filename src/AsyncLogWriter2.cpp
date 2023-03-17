#include "include/AsyncLogWriter.h"
#include "include/CountDownLatch.h"
#include "include/Buffer.h"
#include <thread>
#include <atomic>
#include <condition_variable>
#include <vector>

// using 2 buffers and 2 buffer lists
class AsyncLogWriter2Impl : public AsyncLogWriter {
public:
  AsyncLogWriter2Impl();

  ~AsyncLogWriter2Impl();

  void append(char* content, size_t len) override;

  void start() override;

  void stop() override;

private:
  void threadFunc();

  void putBufToWriteList();

private:
  std::unique_ptr<LargeBuffer> curBuffer_;
  std::unique_ptr<LargeBuffer> backupBuffer_;
  std::vector<std::unique_ptr<LargeBuffer>> buffers_;
  std::atomic<bool> running_;
  std::mutex mtx_;
  std::condition_variable cv_;
  std::shared_ptr<CountDownLatch> latch_;
  std::thread td_;
  int kWaitInterval = 100;
};

AsyncLogWriter2Impl::AsyncLogWriter2Impl() :
running_(false),
latch_(createCountDownLatch(1)),
td_(&AsyncLogWriter2Impl::threadFunc, this)
{
  curBuffer_ = std::make_unique<LargeBuffer>();
  backupBuffer_ = std::make_unique<LargeBuffer>();
  curBuffer_->setZero();
  backupBuffer_->setZero();
  buffers_.reserve(16);
}

AsyncLogWriter2Impl::~AsyncLogWriter2Impl()
{
  AsyncLogWriter2Impl::stop();
}

void AsyncLogWriter2Impl::start()
{
  printf("AsyncLogWriter2: start.\n");
  running_ = true;
  latch_->countDown();
}

void AsyncLogWriter2Impl::stop()
{
  latch_->countDownToZero();
  running_ = false;
  if (td_.joinable()) {
    td_.join();
    printf("AsyncLogWriter2: end.\n");
  }
}

void AsyncLogWriter2Impl::append(char* content, size_t len)
{
  std::lock_guard<std::mutex> guard(mtx_);
  size_t availLen = curBuffer_->avail();
  if (availLen >= len) {
    curBuffer_->append(content, len);
    return;
  }
  printf("%c: current buffer reach max.\n", content[len - 2]);
  putBufToWriteList();
  curBuffer_->append(content, len);
}

void AsyncLogWriter2Impl::threadFunc()
{
  std::vector<std::unique_ptr<LargeBuffer>> buffersToWrite;
  latch_->wait();
  while (running_) {
    { // lock, swap buffers to write
      std::unique_lock<std::mutex> ul(mtx_);
      if (curBuffer_->empty()) {
        cv_.wait_for(ul,std::chrono::milliseconds(kWaitInterval));
      }

      if (curBuffer_->empty()) {
        continue;
      }

      printf("write: push buffer to write list.\n");
      putBufToWriteList();

      printf("write: swap buffer lists.\n");
      std::swap(buffersToWrite, buffers_);
    } // unlock

    assert(!buffersToWrite.empty());

    for (auto& buffer : buffersToWrite) {
      printf("batch: \n%.*s\n", static_cast<int>(buffer->size()), buffer->data());
    }

    { // lock, reuse backupBuffer_
      std::lock_guard<std::mutex> guard(mtx_);
      if (backupBuffer_ == nullptr) {
        printf("write: reuse buffer as backup buffer.\n");
        backupBuffer_ = std::move(buffersToWrite.back());
        buffersToWrite.pop_back();
        backupBuffer_->resetPos();
      }
    } // unlock

    buffersToWrite.clear();
  }
}

void AsyncLogWriter2Impl::putBufToWriteList()
{
  buffers_.emplace_back(std::move(curBuffer_));
  if (backupBuffer_ == nullptr) {
    printf("new buffer created.\n");
    curBuffer_ = std::make_unique<LargeBuffer>();
  } else {
    curBuffer_ = std::move(backupBuffer_);
    curBuffer_->resetPos();
  }
}

AsyncLogWriterPtr createAsyncLogWriter2()
{
  static AsyncLogWriterPtr instance = std::make_shared<AsyncLogWriter2Impl>();
  return instance;
}