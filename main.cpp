#include <thread>
#include <vector>
#include <sstream>
#include "src/Logger.h"

LogLevel GLOG_LEVEL = LogLevel::WARNING;

void func()
{
    LOG_INFO << "good";
    LOG_WARNING << "oh my god" << " shit";
}

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        std::thread td([&](){func();});
        threads.emplace_back(std::move(td));
    }

    for (std::thread& td : threads) {
        td.join();
    }
    return 0;
}
