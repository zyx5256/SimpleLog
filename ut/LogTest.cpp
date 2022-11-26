#include "src/Logger.h"
#include "gtest/gtest.h"

// test info log
TEST(logTest, infoLog)
{
  LOG_INFO << "test info";
}

// test warning log
TEST(logTest, warningLog)
{
  LOG_WARNING << "test warning";
}

// test error log
TEST(logTest, errorLog)
{
  LOG_ERROR << "test error";
}

// test input types: string
TEST(logTest, strTypes)
{
  std::string str = "test str";
  LOG_INFO << str;
}

// test input types: int
TEST(logTest, intTypes)
{
  int i = 11;
  LOG_INFO << i;
}

// test multiple <<
TEST(logTest, multiInsert)
{
  LOG_INFO << "test " << "multiple " << "insert";
}

int main()
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}