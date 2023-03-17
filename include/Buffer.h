#ifndef SIMPLELOG_BUFFER_H
#define SIMPLELOG_BUFFER_H


#include <cstdio>
#include <memory>
#include <algorithm>

template <size_t SIZE>
class Buffer {
public:
  ~Buffer() = default;

  size_t avail() { return SIZE - curPos_; }

  size_t size() { return curPos_; }

  void setZero() { memset(buf_, 0, SIZE); }

  void resetPos() { curPos_ = 0; }

  char* data(){ return buf_; }

  bool empty() { return curPos_ == 0; }

  void append(char* str, size_t len)
  {
    int cpLen = std::min(SIZE - curPos_, len);
    memcpy(buf_ + curPos_, str, cpLen);
    curPos_ += cpLen;
  }

private:
  char buf_[SIZE]{};
  size_t curPos_ = 0;
};

const int kLargeBuffer = 400; // 4000 * 1000;

typedef Buffer<kLargeBuffer> LargeBuffer;


#endif //SIMPLELOG_BUFFER_H
