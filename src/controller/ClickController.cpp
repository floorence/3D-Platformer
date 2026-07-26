#include"ClickController.h"
#include "util/Log.h"
#include <fmt/format.h>

void ClickController::registerClickable(Clickable* clickable) {
    clickables.push_back(clickable);
}

void ClickController::handleClick(float x, float y) {
    Log::log("ClickController", fmt::format("handleClick({}, {})", x, y));
    for (auto& clickable: clickables) {
        clickable->dispatchClick(x, y);
    }
}