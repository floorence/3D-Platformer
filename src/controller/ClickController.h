#ifndef CLICK_CONTROLLER_H
#define CLICK_CONTROLLER_H

#include "Observable.h"
#include "gui/Clickable.h"
#include <string>

class ClickController: public Observable<Clickable> {
public:
    ClickController() = default;

    void handleMouseButton(float x, float y, int action);
    void handleMousePos(float x, float y);
private:
    const std::string TAG = "ClickController";
};

#endif