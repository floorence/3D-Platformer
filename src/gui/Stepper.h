#ifndef STEPPER_H
#define STEPPER_H

#include "gui/Button.h"
#include "gui/Clickable.h"

class Stepper: public GuiElement {
public:
    Stepper() = default;

    bool dispatchMouseEvent(float x, float y, MouseEvent event) override;
    int getCount();
    void setCount(int count);
    void setStepAmount(int amount);
    void setMinMax(int minCount, int maxCount);
    void setColors(glm::vec3 buttonsColor, glm::vec3 textColor);
    void draw(Shader& shader, Shader& fontShader);
private:
    Button decButton;
    Text countText;
    Button incButton;

    int count = 0, stepAmount = 1, min, max;

    void onBoundsChanged() override;
};

#endif
