# Simple Log

---

### 1. Brief Introduction

A simple log module. 

---

### 2. Usage

Set log level as a environment variable.
```bash
$ export LOG_LEVEL=INFO
```

---

Import `Logger.h` file. 

Currently supported API:
- LOG_INFO
- LOG_WARNING
- LOG_ERROR

---

### Example
```c++
#include "SimpleLog/Logger.h"
#include <string>

int main()
{
  std::string str = "info log"
  LOG_INFO << str << " with" << 4 << " insertion.";
  LOG_WARNING << "warning log.";
  LOG_ERROR << "error log.";
}
```