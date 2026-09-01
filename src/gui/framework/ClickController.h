#pragma once

#include "util/Observable.h"
#include "gui/framework/Clickable.h"
#include <string>

class ClickController: public Observable<Clickable> {
public:
    ClickController() = default;

    void handleMouseButton(float x, float y, int action);
    void handleMousePos(float x, float y);
private:
    const std::string TAG = "ClickController";
};
