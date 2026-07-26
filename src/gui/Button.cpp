#include"Button.h"
#include "util/Log.h"

Button::Button(float xu, float yu, float xv, float yv) {
    setBounds(xu, yu, xv, yv);
    useColorInsteadOfTexture = true;
}

void Button::draw(Shader& shader, TextRenderer* textRenderer) {
    Quad::draw(shader);
    textRenderer->drawText(text, xu + padding, yu + padding, xv - xu - padding, 16.0f); // TODO
}

void Button::setOnClick(std::function<void()> callback) {
    onClickCallback = callback;
}

bool Button::dispatchClick(float x, float y) {
    if (x > xu && x < xv && y > yu && y < yv) {
        if (onClickCallback) onClickCallback(); 
        return true;
    }
    return false;
}
