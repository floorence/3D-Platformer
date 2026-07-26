#ifndef CLICK_CONTROLLER_H
#define CLICK_CONTROLLER_H

#include "gui/Clickable.h"
#include <vector>

class ClickController {
public:
    void registerClickable(Clickable* clickable);
    void handleClick(float x, float y);
private:
    std::vector<Clickable*> clickables;
};

#endif