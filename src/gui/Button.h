#ifndef BUTTON_H
#define BUTTON_H

#include "gui/GuiElement.h"
#include "gui/Quad.h"
#include "gui/Text.h"
#include "gui/TextRenderer.h"

class Button: public GuiElement {
public:
    float padding = 2.0f; // TODO

    Button();
    Button(float xu, float yu, float xv, float yv); // TODO: annoying to copy constructor for every guielement

    void setText(std::string text);
    void setBackgroundColor(glm::vec3 color);
    void draw(Shader& shader, Shader& fontShader);
private:
    Quad background;
    Text text;

    void onBoundsChanged() override;
};

#endif