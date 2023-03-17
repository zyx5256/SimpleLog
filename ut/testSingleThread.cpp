#include "gtest/gtest.h"
#include "include/Logger.h"
#include "include/AsyncLogWriter.h"

// test info log
TEST(logTest, basicLogs)
{
  LOG_INFO << "test info";
  LOG_WARNING << "test warning";
  LOG_ERROR << "test error";
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
  std::stringstream ss;
  ss << "string stream";
  LOG_INFO << "test " << 3 << " " << ss.str();
}

// TODO: test api: setOutputFunc
TEST(logTest, setOutputFunc)
{

}

// TODO: test api: setLevel
TEST(logTest, setLevel)
{

}

// TODO: test api: buffer
TEST(logTest, buffer)
{

}

// TODO: other tests


int main()
{
  testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}