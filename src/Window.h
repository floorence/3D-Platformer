#pragma once

#include "WindowListener.h"
#include "controller/SettingsListener.h"
#include "gui/Text.h"
#include <utility>

class Window: public SettingsListener {
public:
	float deltaTime = 0.0f;

    Window(GLFWwindow* window);
    void registerListener(WindowListener* listener);

    void startFrame();
    void endFrame();

    void onSettingsChanged(const Settings& settings);
private:
    GLFWwindow* window;
    std::vector<WindowListener*> listeners;

	float prevFrameStart = 0.0f;
    float currentFrameStart = 0.0f;
    float currentFrameEnd = 0.0f;

	float lastUpdatedInfoText = 0.0f;
	Text performanceText;

	// first: total frame time, second: number of frames. real frame time is the frame time if vsync wasn't on.
	std::pair<float, int> totalFrameTime = std::pair(0.0f, 0);
	std::pair<float, int> totalRealFrameTime = std::pair(0.0f, 0);

    bool fullscreen = false;
    int windowWidth, windowHeight, windowX, windowY;
    const std::string TAG = "Window";

    void notifyListeners(int newWidth, int newHeight);
    void setFullscreen(bool fullscreen);
    std::string formatPerformanceInfo(float frameTime, float realFrameTime);
};