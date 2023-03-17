#include "include/CountDownLatch.h"
#include <mutex>
#include <condition_variable>

class CountDownLatchImpl : public CountDownLatch {
public:
  explicit CountDownLatchImpl(int count);

  ~CountDownLatchImpl() override = default;

  void countDown() override;

  int getCount() override;

  void wait() override;

  void countDownToZero() override;

private:
  std::mutex mtx_;
  std::condition_variable cv_;
  int count_;
};

CountDownLatchImpl::CountDownLatchImpl(int count) : count_(count) {}

void CountDownLatchImpl::countDown()
{
  {
    std::lock_guard<std::mutex> guard(mtx_);
    if (count_ > 0) {
      --count_;
    }
  }
  if (count_ == 0) {
    cv_.notify_all();
  }
}

int CountDownLatchImpl::getCount()
{
  std::lock_guard<std::mutex> guard(mtx_);
  return count_;
}

void CountDownLatchImpl::wait()
{
  std::unique_lock<std::mutex> ul(mtx_);
  cv_.wait(ul, [&]{ return count_ == 0; });
}

void CountDownLatchImpl::countDownToZero()
{
  {
    std::lock_guard<std::mutex> guard(mtx_);
    count_ = 0;
  }
  cv_.notify_all();
}

std::shared_ptr<CountDownLatch> createCountDownLatch(int count)
{
  return std::make_shared<CountDownLatchImpl>(count);
}