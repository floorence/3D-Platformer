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

    background.setBounds(x, y, w, h);
    text.setFontSize(deriveFontSize());
    text.setBounds(x, y, w, h);
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

void Button::draw(Shader& shader, Shader& fontShader) {
    background.draw(shader);
    text.draw(fontShader);
}
