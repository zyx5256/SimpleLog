#include "../src/Logger.h"
#include "gtest/gtest.h"

LogLevel GLOG_LEVEL;

// test input types: string, int
TEST(logTest, inputTypes)
{
  GLOG_LEVEL = LogLevel::INFO;
  std::string str = "test str";
  int i = 11;
  LOG_INFO << str;
  LOG_INFO << i;
}

// test multiple <<
TEST(logTest, multiInsert)
{
  GLOG_LEVEL = LogLevel::INFO;
  LOG_INFO << "test " << "multiple " << "insert";
}

void logAllLevels()
{
  LOG_INFO << "test info";
  LOG_WARNING << "test warning";
  LOG_ERROR << "test error";
}

// test info level
TEST(logTest, info)
{
  GLOG_LEVEL = LogLevel::INFO;
  logAllLevels();
}

// test warning level
TEST(logTest, warning)
{
  GLOG_LEVEL = LogLevel::WARNING;
  logAllLevels();
}

// test error level
TEST(logTest, error)
{
  GLOG_LEVEL = LogLevel::ERROR;
  logAllLevels();
}

int main()
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}