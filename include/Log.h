#ifndef LOG_H
#define LOG_H

#include<string>

namespace Log {
    void log(std::string tag, std::string message);
    void err(std::string tag, std::string message);
    void warn(std::string tag, std::string message);
}

#endif