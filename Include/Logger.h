#ifndef _H_LOGGER
#define _H_LOGGER

#include <string>

enum lvl{
    INFO = 0,
    WARN = 1,
    ERROR = 2
};
class Logger{
public:
    Logger() = delete;
    ~Logger() = default;

    static void log(lvl level, std::string&& msg, const char* where);
    static void dump();


private:
    static std::string str_log;
    static const char* tag[3];
    static bool log_exists;
    static uint32_t errors_logged[2];


};

#endif