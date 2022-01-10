#include "Logger.h"

const map<LogLevel, string> Log::level_strings = {
        {ERROR,   "ERROR"},
        {WARNING, "WARNING"},
        {INFO,    "INFO"},
        {DEBUG,   "DEBUG"},
        {TRACE,   "TRACE"},
};

Log::Log(LogLevel level) : m_level(level) {}

string Log::zfill(string str, unsigned short length) {
    if (length <= str.length()) {
        return str;
    }

    length -= str.length();
    while (length-- > 0) {
        str.insert(str.begin(), '0');
    }
    return str;
}

ostringstream &Log::Get(bool prefix) {
    if (prefix) {
        time_t now_time = time(nullptr);
        tm *now = localtime(&now_time);
        os << "[" << 1900 + now->tm_year << "/" << zfill(to_string(1 + now->tm_mon), 2) << "/"
           << zfill(to_string(now->tm_mday), 2) << " ";
        os << zfill(to_string(now->tm_hour), 2) << ":" << zfill(to_string(now->tm_min), 2) << ":"
           << zfill(to_string(now->tm_sec), 2);
        os << "] " << level_strings.at(m_level) << ": \t";
    }
    return os;
}

Log::~Log() {
    if (m_level >= enabled_level) {
        fprintf(stderr, "%s", os.str().c_str());
        fflush(stderr);
    }
}
