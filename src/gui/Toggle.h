#ifndef TOGGLE_H
#define TOGGLE_H

#include "gui/Button.h"
#include "gui/Quad.h"

class Toggle: public Button {
public:
    float padding = 2.0f; // TODO

    Toggle() = default;
    Toggle(float xu, float yu, float xv, float yv);

    bool getIsOn();
    void setIsOn(bool on);
    void setColors(glm::vec3 innerColor, glm::vec3 outerColor);
    void setBounds(float xu, float yu, float xv, float yv) override;
    void draw(Shader& shader) override;
private:
    bool on = false;
    Quad innerSquare;

    void updateInnerSquare();
    using Button::drawWithText;
};

#endif
