#pragma once

#include <string>

namespace Log {
    void log(std::string tag, std::string message);
    void err(std::string tag, std::string message);
    void warn(std::string tag, std::string message);
}
