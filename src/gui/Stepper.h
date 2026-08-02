#ifndef STEPPER_H
#define STEPPER_H

#include "gui/Button.h"
#include "gui/Clickable.h"

class Stepper: public GuiElement {
public:
    Stepper();

    bool dispatchClick(float x, float y) override;
    int getCount();
    void setCountAndMinMax(int count, int minCount, int maxCount);
    void setColors(glm::vec3 buttonsColor, glm::vec3 textColor);
    void draw(Shader& shader, Shader& fontShader);
private:
    Button decButton;
    Text countText;
    Button incButton;

    int count = 0, min, max;

    void onBoundsChanged() override;
    void onCountChanged();
};

#endif
