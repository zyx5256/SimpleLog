#include "Log.h"
#include <unordered_map>
#include <sstream>

namespace {
std::unordered_map<Level, std::string> LOG_NAME_MAP {
    {Level::INFO, "INFO"},
    {Level::WARNING, "WARNING"},
    {Level::ERROR, "ERROR"},
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
    ss << "[" << LOG_NAME_MAP[level] << "]" << content;
    printf("%s\n", &ss.str()[0]);
}
