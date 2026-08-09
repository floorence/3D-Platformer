#ifndef TOGGLE_H
#define TOGGLE_H

#include "gui/Button.h"
#include "gui/Quad.h"

class Toggle: public GuiElement {
public:
    Toggle();

    bool getIsOn();
    void setIsOn(bool on);
    void setColors(glm::vec3 innerColor, glm::vec3 outerColor);
    void draw() override;
private:
    bool on = false;
    float padding = 2.0f;
    Quad outerRect;
    Quad innerSquare;

    void updateInnerSquare();
    void onBoundsChanged() override;
};

#endif
