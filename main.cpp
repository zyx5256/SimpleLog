#include "src/Logger.h"
#include <thread>
#include <vector>
#include <cstdlib>


void func()
{
  LOG_INFO << "good";
  LOG_WARNING << "oh my god" << " shit";
}

int main()
{
  setenv("LOG_LEVEL", "WARNING", 1);
  printf("log level set to %s\n", std::getenv("LOG_LEVEL"));
  std::vector<std::thread> threads;
  for (int i = 0; i < 10; ++i) {
    std::thread td([&]() { func(); });
    threads.emplace_back(std::move(td));
  }

  for (std::thread& td: threads) {
    td.join();
  }
  return 0;
}
