#include "Window.h"
#include <GLFW/glfw3.h>

Window::Window(float windowWidth, float windowHeight) {
	performanceText.setBounds(windowWidth - 200, 10, 200, 100);
	performanceText.setFontSize(16);
	performanceText.setCenterText(false);
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
        
        performanceText.setText(formatPerformanceInfo(avgFrameTime, avgRealFrameTime)); 
        totalFrameTime = std::pair(0.0f, 0);
        totalRealFrameTime = std::pair(0.0f, 0);
    }

    performanceText.draw();
    prevFrameStart = currentFrameStart;
}

std::string Window::formatPerformanceInfo(float frameTime, float realFrameTime) {
	int fps = 1 / frameTime;
	int realFps = 1 / realFrameTime;

	return fmt::format("FPS: {}  |  {}\nframe time: {:.3f}  |  {:.3f}", fps, realFps, frameTime * 1000, realFrameTime * 1000);
}
