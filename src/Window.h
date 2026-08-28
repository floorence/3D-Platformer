#pragma once

#include "gui/Text.h"
#include <utility>

class Window {
public:
	float deltaTime = 0.0f;

    Window(float windowWidth, float windowHeight);

    void startFrame();
    void endFrame();
private:
	float prevFrameStart = 0.0f;
    float currentFrameStart = 0.0f;
    float currentFrameEnd = 0.0f;

	float lastUpdatedInfoText = 0.0f;
	Text performanceText;

	// first: total frame time, second: number of frames. real frame time is the frame time if vsync wasn't on.
	std::pair<float, int> totalFrameTime = std::pair(0.0f, 0);
	std::pair<float, int> totalRealFrameTime = std::pair(0.0f, 0);

    std::string formatPerformanceInfo(float frameTime, float realFrameTime);
};