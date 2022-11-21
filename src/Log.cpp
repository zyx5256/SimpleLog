#include "Log.h"
#include <unordered_map>
#include <sstream>

namespace {
std::unordered_map<Level, std::string> LOG_NAME_MAP {
    {Level::INFO, "INFO"},
    {Level::WARNING, "WARNING"},
    {Level::ERROR, "ERROR"},
};

class FuncTraceHelper {
public:
    FuncTraceHelper()
    {
        printf("[%d][%s] in\n", __LINE__, __FUNCTION__);
    }

    ~FuncTraceHelper()
    {
        printf("[%d][%s] out\n", __LINE__, __FUNCTION__);
    }
};
}

void LOG::INFO(const std::string& content)
{
    log(Level::INFO, content);
}

void LOG::WARNING(const std::string& content)
{
    log(Level::WARNING, content);
}

void LOG::ERROR(const std::string& content)
{
    log(Level::ERROR, content);
}

void LOG::log(const Level& level, const std::string& content)
{
    if (level < LOG_LEVEL) {
        return;
    }
    std::stringstream ss;
    ss << "[" << LOG_NAME_MAP[level] << "]" << "[" << __FUNCTION__ << "]" << content;
    printf("%s\n", &ss.str()[0]);
}

void LOG::FUNC_TRACE_INFO()
{
    FuncTraceHelper();
}
