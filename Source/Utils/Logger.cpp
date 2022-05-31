#include <iostream>
#include <ctime>
#include <iomanip>
#include <fstream>

#include "Globals.h"
#include "Logger.h"

std::string Logger::str_log = "*============================= LOGGED SESSION =============================*\n";
const char* Logger::tag[3] = {"[INF]","[WRN]","[ERR]"};
bool Logger::log_exists = false;
uint32_t Logger::errors_logged[2] = {0};

void Logger::log(lvl level, std::string&& msg, const char* where) {
    // Logs information
    if(!log_exists)
        log_exists = true;

    if(level != INFO){
        if(level == WARN)
            errors_logged[WARN-1]++;
        if(level == ERROR)
            errors_logged[ERROR-1]++;

        std::cout << "Errors: " << errors_logged[1] << " / Warnings: " << errors_logged[0] << "\n";
    }


    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::stringstream s;
    s << std::put_time(&tm, "   %Y-%m-%d @ %H:%M:%S   ");
    s << "|" << where << "| ";

    msg.insert(0, s.str());
    msg.insert(0, tag[level]);
    msg.append("\n");

    str_log.append(msg);



}

void Logger::dump() {
    // Dumps log to output directory (called on harness detachment)
    if(log_exists){
        std::ofstream f;
        f.open(DUMP_LOC+"log.txt");
        f << str_log;
        f.close();
    }
}




