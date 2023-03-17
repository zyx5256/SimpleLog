# Simple Log

---

### 1. Brief Introduction

- A high performance log module. 
- Supports multithreaded logging.
- Log write can be asynchronized with log generation.
- The main idea is from https://github.com/chenshuo/muduo.

---

### 2. Usage

Set log level as a environment variable:
```bash
$ export LOG_LEVEL=INFO
```
Or in your code:

```c
Logger::setLevel(LogLevel::INFO);
```
---

Currently supported APIs:
- LOG_INFO
- LOG_WARNING
- LOG_ERROR

---

### Example 1: Sync Logging
```c++
#include "include/Logger.h"
#include <string>

int main()
{
  LOG_INFO << "info log" << " with" << 4 << " insertion.";
  LOG_WARNING << "warning log.";
  LOG_ERROR << "error log.";
}
```

### Example 2: Async Logging
```c++
#include "include/Logger.h"
#include "include/AsyncLogWriter.h"
#include <string>

AsyncLogWriterPtr g_asyncLogWriter = createAsyncLogWriter2();

int main()
{
  Logger::setLevel(LogLevel::INFO);
  Logger::setOutputFunc(
    [&](char* content, size_t len) { 
      g_asyncLogWriter->append(content, len); 
    });
  
  g_asyncLogWriter->start();
  
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 20));
    threads.emplace_back([&]{ LOG_INFO << "info log"; });
  }

  for (std::thread& td: threads) {
    td.join();
  }
  
  sleep(1);
  g_asyncLogWriter->stop();
  return 0;
}
```