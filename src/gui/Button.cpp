#include"Button.h"
#include "util/Log.h"

Button::Button() {
    background.useColorInsteadOfTexture = true;
}

void Button::onBoundsChanged() {
    if (w < 0) {
        w = h;
    } else if (h < 0) {
        h = w;
    }

    int fontSize = h * 2 / 3;
    background.setBounds(x, y, w, h);
    text.setFontSize(fontSize);
    text.setBounds(x, y, w, h);
}

void Button::setText(std::string text) {
    this->text.setText(text);
}

void Button::setBackgroundColor(glm::vec3 color) {
    background.color = color;
}

void Button::draw(Shader& shader, Shader& fontShader) {
    background.draw(shader);
    text.draw(fontShader);
}
