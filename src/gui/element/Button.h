#pragma once

#include "gui/element/GuiElement.h"
#include "gui/framework/Quad.h"
#include "gui/framework/Text.h"

class Button: public GuiElement {
public:
    Button() = default;

    void setText(std::string text);
    void setBackgroundColor(glm::vec3 color);
    void draw() override;

    void onHover() override;
    void onHoverOff() override;
    // public since other gui elements who use Button might need to call these
    float getUnboundWidth(float h) override;
    float getUnboundHeight(float w) override;
private:
    Quad background;
    Text text;

    const float TEXT_TO_EDGE_MARGIN = 10.0f;

    void onBoundsChanged() override;
    int deriveFontSize(float h);
};
