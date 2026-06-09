#include"Log.h"
#include <ctime>
#include <iomanip>
#include <chrono>
#include <iostream>

std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();

    std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm* local_tm = std::localtime(&time_t_now); // Use localtime_s or localtime_r for thread-safety

    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() % 1000;

    return Log::oss(std::put_time(local_tm, "%Y-%m-%d %H:%M:%S"), ".", std::setfill('0'), std::setw(3), millis);
}

void Log::log(std::string tag, std::string message) {
    std::cout << getTimestamp() << " " << tag << ": " << message << std::endl;
}

void Log::err(std::string tag, std::string message) {
    std::cerr << getTimestamp() << " " << tag << ": " << message << std::endl;
}