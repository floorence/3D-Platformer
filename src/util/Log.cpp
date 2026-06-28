#include"Log.h"
#include <fmt/base.h>
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/color.h>

std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();

    std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm* local_tm = std::localtime(&time_t_now); // not thread safe btw but idc rn

    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;

    return fmt::format("{:%m-%d %H:%M:%S}.{:03d}", *local_tm, millis);
}

void Log::log(std::string tag, std::string message) {
    fmt::print("{} [I] {}: {}\n", getTimestamp(), tag, message);
}

void Log::err(std::string tag, std::string message) {
    fmt::print(fg(fmt::color::red), "{} [E] {}: {}\n", getTimestamp(), tag, message);
}

void Log::warn(std::string tag, std::string message) {
    fmt::print(fg(fmt::color::gold), "{} [W] {}: {}\n", getTimestamp(), tag, message);
}