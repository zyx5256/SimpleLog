#include "../src/Logger.h"
#include "gtest/gtest.h"

LogLevel GLOG_LEVEL;

void logAllLevels()
{
    LOGGER.INFO("test info");
    LOGGER.WARNING("test warning");
    LOGGER.ERROR("test error");
}

// test info level
TEST(Log_test, info)
{
    GLOG_LEVEL = LogLevel::INFO;
    logAllLevels();
}

// test warning level
TEST(Log_test, warning)
{
    GLOG_LEVEL = LogLevel::WARNING;
    logAllLevels();
}

// test error level
TEST(Log_test, error)
{
    GLOG_LEVEL = LogLevel::ERROR;
    logAllLevels();
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}