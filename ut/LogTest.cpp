#include "../src/Log.h"
#include "gtest/gtest.h"

Level LOG_LEVEL;

void logAllLevels()
{
    LOG::INFO("test info");
    LOG::WARNING("test warning");
    LOG::ERROR("test error");
}

// test info level
TEST(Log_test, info)
{
    LOG_LEVEL = Level::INFO;
    logAllLevels();
}

// test warning level
TEST(Log_test, warning)
{
    LOG_LEVEL = Level::WARNING;
    logAllLevels();
}

// test error level
TEST(Log_test, error)
{
    LOG_LEVEL = Level::ERROR;
    logAllLevels();
}

int main()
{
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}