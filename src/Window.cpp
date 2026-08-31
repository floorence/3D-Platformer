#include "Window.h"
#include "util/Log.h"
#include <GLFW/glfw3.h>

Window::Window(GLFWwindow* window)
    : window(window) 
{
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
}

void Window::startFrame() {
    currentFrameStart = glfwGetTime();
    deltaTime = currentFrameStart - prevFrameStart;
}

void Window::endFrame() {
    currentFrameEnd = glfwGetTime();
    totalFrameTime.first += deltaTime;
    totalFrameTime.second++;
    totalRealFrameTime.first += currentFrameEnd - currentFrameStart;
    totalRealFrameTime.second++;

    if (currentFrameEnd - lastUpdatedInfoText >= 1.0f) {
        lastUpdatedInfoText = currentFrameEnd;
        float avgFrameTime = totalFrameTime.first / totalFrameTime.second;
        float avgRealFrameTime = totalRealFrameTime.first / totalRealFrameTime.second;
        
        formatPerformanceInfo(avgFrameTime, avgRealFrameTime); 
        totalFrameTime = std::pair(0.0f, 0);
        totalRealFrameTime = std::pair(0.0f, 0);
    }

    prevFrameStart = currentFrameStart;
}

void Window::onSettingsChanged(const Settings& settings) {
    int width, height;
    settings.graphics.getResolution(&width, &height);
    if (width != windowWidth || height != windowHeight) {
        Log::log(TAG, fmt::format("Changing window size: {}x{} -> {}x{}", windowWidth, windowHeight, width, height));

        glfwSetWindowSize(window, width, height);
        setSizeAndNotify(width, height);
    }
    setFullscreen(settings.graphics.fullscreen.value, width, height);
}

void Window::setSizeAndNotify(int newWidth, int newHeight) {
    windowWidth = newWidth;
    windowHeight = newHeight;
    for (auto& listener: listeners) {
        listener->onWindowSizeChanged(windowWidth, windowHeight);
    }
}

void Window::setFullscreen(bool fullscreen, int windowedWidth, int windowedHeight) {
    if (this->fullscreen == fullscreen) return;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) return;

    if (fullscreen) {
        glfwGetWindowPos(window, &savedX, &savedY);
        // glfwGetWindowSize(window, &savedW, &savedH);

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        // windowWidth = mode->width;
        // windowHeight = mode->height;
        // notifyListeners();

        Log::log(TAG, fmt::format("Switched to fullscreen: {}x{} @ {}Hz", mode->width, mode->height, mode->refreshRate));
    } else {
        glfwSetWindowMonitor(window, nullptr, savedX, savedY, windowedWidth, windowedHeight, 0);
        setSizeAndNotify(windowedWidth, windowedHeight);
        
        Log::log(TAG, fmt::format("Switched to windowed mode: {}x{}", windowedWidth, windowedHeight));
    }

    // glfwGetWindowSize(window, &windowWidth, &windowHeight);
    // Log::log(TAG, fmt::format("New window size: {}x{}", windowWidth, windowHeight));

    this->fullscreen = fullscreen;
}

void Window::formatPerformanceInfo(float frameTime, float realFrameTime) {
	int fps = 1 / frameTime;
	int realFps = 1 / realFrameTime;

	performanceInfo = fmt::format("FPS: {}  |  {}\nframe time: {:.3f}  |  {:.3f}", fps, realFps, frameTime * 1000, realFrameTime * 1000);
}
