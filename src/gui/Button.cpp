#include"Button.h"
#include "texture/FontTexture.h"
#include "util/Log.h"
#include <fmt/color.h>

void Button::onBoundsChanged() {
    background.setBounds(x, y, w, h);
    text.setFontSize(deriveFontSize(h));
    text.setBounds(x, y, w, h);
}

float Button::getUnboundWidth(float h) {
    Log::log("Button", fmt::format("getUnboundWidth: text: {} fontSize: {}", text.getText(), deriveFontSize(h)));
    std::pair<float, float> textSize = Globals::Font->getSize(text.getText(), deriveFontSize(h));
    return TEXT_TO_EDGE_MARGIN * 2 + textSize.first;
}

float Button::getUnboundHeight(float w) {
    return w / 4;
}

void Button::onHover() {
    // Log::log("Button", fmt::format("{} onHover()", text.getText()));
    background.tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.1f);
}

void Button::onHoverOff() {
    // Log::log("Button", fmt::format("{} onHoverOff()", text.getText()));
    background.tintColor = glm::vec4(0.0f);
}

int Button::deriveFontSize(float h) {
    if (text.getText().size() == 1) { // text is being used as icon, make it bigger
        return h * 3/4;
    } else {
        return h / 2;
    }
}

void Button::setText(std::string text) {
    this->text.setText(text);
    invalidateBounds();
}

void Button::setBackgroundColor(glm::vec3 color) {
    background.setColor(color);
}

void Button::draw() {
    background.draw();
    text.draw();
}
