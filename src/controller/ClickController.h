#ifndef CLICK_CONTROLLER_H
#define CLICK_CONTROLLER_H

#include "gui/Clickable.h"
#include <string>
#include <vector>

class ClickController {
public:
    void registerClickable(Clickable* clickable);
    void registerClickables(const std::vector<Clickable*>& clickables);
    void handleMouseButton(float x, float y, int action);
    void handleMousePos(float x, float y);
private:
    std::vector<Clickable*> clickables;

    const std::string TAG = "ClickController";
};

#endif