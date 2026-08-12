#ifndef STEPPER_H
#define STEPPER_H

#include "gui/Button.h"
#include "gui/Clickable.h"
#include "gui/IntGuiElement.h"

class Stepper: public IntGuiElement {
public:
    Stepper();

    bool dispatchMouseEvent(float x, float y, MouseEvent event) override;
    int getData() override;
    void setData(int data) override;
    void setStepAmount(int amount);
    void setMinMax(int minCount, int maxCount);
    void setUnits(std::string units);
    void setColors(glm::vec3 buttonsColor, glm::vec3 textColor);
    void draw() override;
private:
    Button decButton;
    Text countText;
    Button incButton;
    std::string units = "";

    int count = 0, stepAmount = 1, min, max;

    void onBoundsChanged() override;
    float getUnboundWidth(float h) override { return h * 4; };
    float getUnboundHeight(float w) override { return w / 4; };
};

#endif
