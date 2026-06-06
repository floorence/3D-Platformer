#include"Log.h"

std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    
    std::tm local_tm = *std::localtime(&time_now);
    
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_tm);
    
    return std::string(buffer);
}

void Log::log(std::string tag, std::string message) {
    std::cout << getTimestamp() << " " << tag << ": " << message << std::endl;
}