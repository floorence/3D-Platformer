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

        windowWidth = width;
        windowHeight = height;
        glfwSetWindowSize(window, windowWidth, windowHeight);
        notifyListeners(width, height);
    }
    setFullscreen(settings.graphics.fullscreen.value);
}

void Window::notifyListeners(int newWidth, int newHeight) {
    for (auto& listener: listeners) {
        listener->onWindowSizeChanged(newWidth, newHeight);
    }
}

void Window::setFullscreen(bool fullscreen) {
    if (this->fullscreen == fullscreen) return;
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) return;

    if (fullscreen) {
        glfwGetWindowPos(window, &savedX, &savedY);
        glfwGetWindowSize(window, &savedW, &savedH);

        // use optimal refresh rate if monitor supports the resolution
        int mode_count;
        const GLFWvidmode* modes = glfwGetVideoModes(monitor, &mode_count);
        int refreshRate = 60;

        for (int i = 0; i < mode_count; i++) {
            if (modes[i].width == windowWidth && modes[i].height == windowHeight) {
                refreshRate = modes[i].refreshRate;
                break;
            }
        }

        glfwSetWindowMonitor(window, monitor, 0, 0, windowWidth, windowHeight, refreshRate);
        Log::log(TAG, fmt::format("Switched to fullscreen: {}x{} @ {}Hz", windowWidth, windowHeight, refreshRate));
    } else {
        glfwSetWindowMonitor(window, nullptr, savedX, savedY, savedW, savedH, 0);
        Log::log(TAG, fmt::format("Switched to windowed mode: {}x{}", savedW, savedH));
    }

    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    Log::log(TAG, fmt::format("New window size: {}x{}", windowWidth, windowHeight));
    notifyListeners(windowWidth, windowHeight);

    this->fullscreen = fullscreen;
}

void Window::formatPerformanceInfo(float frameTime, float realFrameTime) {
	int fps = 1 / frameTime;
	int realFps = 1 / realFrameTime;

	performanceInfo = fmt::format("FPS: {}  |  {}\nframe time: {:.3f}  |  {:.3f}", fps, realFps, frameTime * 1000, realFrameTime * 1000);
}
