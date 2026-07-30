#include"Button.h"
#include "util/Log.h"

Button::Button() {
    background.useColorInsteadOfTexture = true;
    text.setFontSize(16);
}

Button::Button(float xu, float yu, float xv, float yv): Button() {
    setCorners(xu, yu, xv, yv);
}

void Button::onBoundsChanged() {
    background.setBounds(x, y, w, h);
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
