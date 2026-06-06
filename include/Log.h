#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <sstream>
#include <chrono>

namespace Log {
    void log(std::string tag, std::string message);

    template<typename... Args>
    std::string oss(const Args&... args) {
        std::ostringstream oss;
        (oss << ... << args);
        return oss.str();
    }
}

#endif