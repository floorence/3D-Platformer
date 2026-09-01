#include "ClickController.h"
#include <GLFW/glfw3.h>
#include "gui/framework/Clickable.h"
#include "util/Log.h"
#include <fmt/format.h>

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

    for (auto& clickable: listeners) {
        clickable->dispatchMouseEvent(x, y, event);
    }
}

void ClickController::handleMousePos(float x, float y) {
    for (auto& clickable: listeners) {
        clickable->dispatchMouseEvent(x, y, MouseEvent::Hover);
    }
}