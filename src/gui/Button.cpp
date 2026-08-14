#include"Button.h"
#include "util/Log.h"
#include <fmt/color.h>

Button::Button() {
    background.useColorInsteadOfTexture = true;
}

void Button::onBoundsChanged() {
    background.setBounds(x, y, w, h);
    text.setFontSize(deriveFontSize());
    text.setBounds(x, y, w, h);
}

void Button::onHover() {
    // Log::log("Button", fmt::format("{} onHover()", text.getText()));
    background.tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.1f);
}

void Button::onHoverOff() {
    // Log::log("Button", fmt::format("{} onHoverOff()", text.getText()));
    background.tintColor = glm::vec4(0.0f);
}

int Button::deriveFontSize() {
    if (text.getText().size() == 1) { // text is being used as icon, make it bigger
        return h * 3/4;
    } else {
        return h / 2;
    }
}

void Button::setText(std::string text) {
    this->text.setText(text);
    this->text.setFontSize(deriveFontSize());
}

void Button::setBackgroundColor(glm::vec3 color) {
    background.color = color;
}

void Button::draw() {
    background.draw();
    text.draw();
}
