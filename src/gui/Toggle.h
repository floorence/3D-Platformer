#ifndef TOGGLE_H
#define TOGGLE_H

#include "gui/Button.h"
#include "gui/Quad.h"

class Toggle: public GuiElement {
public:
    float padding = 2.0f; // TODO

    Toggle() = default;
    Toggle(float xu, float yu, float xv, float yv);

    bool getIsOn();
    void setIsOn(bool on);
    void setColors(glm::vec3 innerColor, glm::vec3 outerColor);
    void draw(Shader& shader);
private:
    bool on = false;
    Quad outerRect;
    Quad innerSquare;

    void updateInnerSquare();
    void onBoundsChanged() override;
};

#endif
