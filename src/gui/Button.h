#ifndef BUTTON_H
#define BUTTON_H

#include "gui/GuiElement.h"
#include "gui/Quad.h"
#include "gui/TextRenderer.h"

class Button: public GuiElement {
public:
    std::string text = "";
    float padding = 2.0f; // TODO

    Button() = default;
    Button(float xu, float yu, float xv, float yv); // TODO: annoying to copy constructor for every guielement

    void setBackgroundColor(glm::vec3 color);
    void draw(Shader& shader, TextRenderer& textRenderer);
private:
    Quad background;

    void onBoundsChanged() override;
};

#endif