#pragma once

#include "Observable.h"
#include "WindowListener.h"
#include "controller/SettingsListener.h"
#include <GLFW/glfw3.h>
#include <utility>

class Window: public SettingsListener, public Observable<WindowListener> {
public:
	float deltaTime = 0.0f;
    std::string performanceInfo;

    Window(GLFWwindow* window);

    void startFrame();
    void endFrame();

    void setSizeAndNotify(int newWidth, int newHeight);
    void onSettingsChanged(const Settings& settings);
private:
    GLFWwindow* window;

	float prevFrameStart = 0.0f;
    float currentFrameStart = 0.0f;
    float currentFrameEnd = 0.0f;
	float lastUpdatedInfoText = 0.0f;

	// first: total frame time, second: number of frames. real frame time is the frame time if vsync wasn't on.
	std::pair<float, int> totalFrameTime = std::pair(0.0f, 0);
	std::pair<float, int> totalRealFrameTime = std::pair(0.0f, 0);

    bool fullscreen = false;
    int windowWidth, windowHeight;
    int savedX, savedY;
    const std::string TAG = "Window";

    void getResolution(std::string res, int* width, int* height);
    void setFullscreen(bool fullscreen, int windowedWidth, int windowedHeight);
    void formatPerformanceInfo(float frameTime, float realFrameTime);
};