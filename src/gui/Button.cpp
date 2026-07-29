#include"Button.h"
#include "util/Log.h"

Button::Button(float xu, float yu, float xv, float yv) {
    setCorners(xu, yu, xv, yv);
    background.useColorInsteadOfTexture = true;
}

void Button::onBoundsChanged() {
    background.setBounds(x, y, w, h);
}

void Button::setBackgroundColor(glm::vec3 color) {
    background.color = color;
}

void Button::draw(Shader& shader, TextRenderer& textRenderer) {
    background.draw(shader);
    textRenderer.drawText(text, x + padding, y + padding, x + w - padding, 16.0f); // TODO
}
