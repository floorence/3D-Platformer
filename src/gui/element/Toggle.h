#ifndef TOGGLE_H
#define TOGGLE_H

#include "IntGuiElement.h"
#include "gui/framework/Quad.h"

class Toggle: public IntGuiElement {
public:
    Toggle();

    int getData() override;
    void setData(int data) override;
    void setColors(glm::vec3 innerColor, glm::vec3 outerColor);
    void draw() override;
private:
    bool on = false;
    float padding = 2.0f;
    Quad outerRect;
    Quad innerSquare;

    void updateInnerSquare();
    void onBoundsChanged() override;
    float getUnboundWidth(float h) override { return h * 2; };
    float getUnboundHeight(float w) override { return w / 2; };
};

#endif
