#ifndef BUTTON_H
#define BUTTON_H

#include "gui/GuiElement.h"
#include "gui/Quad.h"
#include "gui/Text.h"

class Button: public GuiElement {
public:
    Button();

    void setText(std::string text);
    void setBackgroundColor(glm::vec3 color);
    void draw(Shader& shader, Shader& fontShader);
private:
    Quad background;
    Text text;

    void onBoundsChanged() override;
    int deriveFontSize();
};

#endif