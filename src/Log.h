#ifndef MULTITHREADPROJ_LOG_H
#define MULTITHREADPROJ_LOG_H


#include <string>

enum Level {
INFO,
WARNING,
ERROR,
};

extern Level LOG_LEVEL;

class LOG {
public:
    LOG() = delete;
    static void INFO(const std::string& content);
    static void WARNING(const std::string& content);
    static void ERROR(const std::string& content);

private:
    static void log(const Level& level, const std::string& content);
};


#endif //MULTITHREADPROJ_LOG_H
