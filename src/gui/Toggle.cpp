#include"Toggle.h"
#include "util/Log.h"

Toggle::Toggle(float xu, float yu, float xv, float yv) {
    if (yv - yu >= xv - xu) {
        Log::err("Toggle", "Toggle must be wider than it is tall! bounds not initialized.");
        return;
    }
    setBounds(xu, yu, xv, yv);
    useColorInsteadOfTexture = true;
    innerSquare.useColorInsteadOfTexture = true;

    setOnClick([this]() {
        on = !on;
        setIsOn(on);
    });
}

bool Toggle::getIsOn() {
    return on;
}

void Toggle::setIsOn(bool on) {
    this->on = on;
    updateInnerSquare();
}

void Toggle::setColors(glm::vec3 innerColor, glm::vec3 outerColor) {
    color = outerColor;
    innerSquare.color = innerColor;
}

void Toggle::setBounds(float xu, float yu, float xv, float yv) {
    Quad::setBounds(xu, yu, xv, yv);
    updateInnerSquare();
}

void Toggle::draw(Shader& shader) {
    Quad::draw(shader);
    innerSquare.draw(shader);
}

void Toggle::updateInnerSquare() {
    float size = yv - yu - padding * 2;
    if (on) {
        float start = xv - padding - size;
        innerSquare.setBounds(start, yu + padding, start + size, yv - padding);
    } else {
        innerSquare.setBounds(xu + padding, yu + padding, xu + padding + size, yv - padding);
    }
}