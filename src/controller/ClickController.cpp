#include<GLFW/glfw3.h>
#include"ClickController.h"
#include "gui/Clickable.h"
#include "util/Log.h"
#include <fmt/format.h>

void ClickController::registerClickable(Clickable* clickable) {
    clickables.push_back(clickable);
}

void ClickController::registerClickables(const std::vector<Clickable*>& clickables) {
    this->clickables.insert(this->clickables.end(), clickables.begin(), clickables.end());
}

void ClickController::handleMouseButton(float x, float y, int action) {
    Log::log(TAG, fmt::format("handleMouseButton({}, {}, {})", x, y, action));
    MouseEvent event;
    if (action == GLFW_PRESS) {
        event = MouseEvent::Down;
    } else if (action == GLFW_RELEASE) {
        event = MouseEvent::Up;
    } else {
        Log::warn(TAG, fmt::format("Unrecognized mouse action: {}", action));
    }

    for (auto& clickable: clickables) {
        clickable->dispatchMouseEvent(x, y, event);
    }
}

void ClickController::handleMousePos(float x, float y) {
    for (auto& clickable: clickables) {
        clickable->dispatchMouseEvent(x, y, MouseEvent::Hover);
    }
}