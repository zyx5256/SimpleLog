#include "include/Logger.h"
#include <thread>
#include <vector>
#include "include/AsyncLogWriter.h"

AsyncLogWriterPtr GWRITER = createAsyncLogWriter();

void func(int i)
{
  LOG_INFO << "good " << i ;
//  LOG_WARNING << "oh my god" << " shit";
}

int main(int argc, char* argv[])
{
  Logger::setLevel(LogLevel::INFO);
  std::vector<std::thread> threads;
  for (int i = 0; i < atoi(argv[1]); ++i) {
    threads.emplace_back(func, i);
  }

  for (std::thread& td: threads) {
    td.join();
  }
  GWRITER->start();
  GWRITER->stop();
  return 0;
}
