#ifndef BUTTON_H
#define BUTTON_H

#include "gui/GuiElement.h"
#include "gui/Quad.h"
#include "gui/Text.h"

class Button: public GuiElement {
public:
    Button() = default;

    void setText(std::string text);
    void setBackgroundColor(glm::vec3 color);
    void draw() override;

    void onHover() override;
    void onHoverOff() override;
private:
    Quad background;
    Text text;

    void onBoundsChanged() override;
    int deriveFontSize();
};

#endif