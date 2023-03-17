#include "include/Logger.h"
#include "include/AsyncLogWriter.h"
#include <thread>
#include <vector>
#include <unistd.h>
#include <random>

AsyncLogWriterPtr GWRITER = createAsyncLogWriter2();

void func(int i)
{
  LOG_INFO << "good " << i ;
//  LOG_WARNING << "oh my god" << " shit";
}

int main()
{
  Logger::setOutputFunc([&](char* content, size_t len) { GWRITER->append(content, len); });
  GWRITER->start();
  Logger::setLevel(LogLevel::INFO);
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 50));
    threads.emplace_back(func, i);
  }

  for (std::thread& td: threads) {
    td.join();
  }
  sleep(1);
  GWRITER->stop();
  return 0;
}
