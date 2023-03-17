#ifndef SIMPLELOG_COUNTDOWNLATCH_H
#define SIMPLELOG_COUNTDOWNLATCH_H

#include <memory>

class CountDownLatch {
public:
  virtual ~CountDownLatch() = default;

  virtual void countDown() = 0;

  virtual int getCount() = 0;

  virtual void wait() = 0;

  virtual void countDownToZero() = 0;
};

std::shared_ptr<CountDownLatch> createCountDownLatch(int count);


#endif //SIMPLELOG_COUNTDOWNLATCH_H
