#pragma once

#include <map>
#include <sstream>

using namespace std;

// todo: get -D and use config.h at compile time to decide what to log.
// Log, version 0.1: a simple logging class
// Taken and refactored from Petru Marginean's article in Dr. Dobbs - https://www.drdobbs.com/cpp/logging-in-c/201804215
enum LogLevel {
    ERROR = 5,
    WARNING = 4,
    INFO = 3,
    DEBUG = 2,
    TRACE = 1
};

class Log {
public:
    static const map<LogLevel, string> level_strings;

    Log(LogLevel level = INFO);

    virtual ~Log();

    static string zfill(string str, int length);

    ostringstream &Get();

    static const LogLevel enabled_level = INFO;

private:
    LogLevel m_level;

    ostringstream os;
};
