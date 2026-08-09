#include"Toggle.h"
#include "util/Log.h"

Toggle::Toggle() {
    outerRect.useColorInsteadOfTexture = true;
    innerSquare.useColorInsteadOfTexture = true;

    setOnClick([this]() {
        on = !on;
        setData(on);
    });
}

int Toggle::getData() {
    return on;
}

void Toggle::setData(int data) {
    this->on = data;
    updateInnerSquare();
}

void Toggle::setColors(glm::vec3 innerColor, glm::vec3 outerColor) {
    outerRect.color = outerColor;
    innerSquare.color = innerColor;
}

void Toggle::onBoundsChanged() {
    if (w < 0) {
        w = h * 2;
    } else {
        h = w / 2;
    }
    if (h >= w) {
        Log::err("Toggle", "Toggle must be wider than it is tall! bounds were not fully initialized and bad things will happen.");
        return;
    }
    padding = h / 10;
    outerRect.setBounds(x, y, w, h);
    updateInnerSquare();
}

void Toggle::draw() {
    outerRect.draw();
    innerSquare.draw();
}

void Toggle::updateInnerSquare() {
    float size = h - padding * 2;
    if (on) {
        float start = x + w - padding - size;
        innerSquare.setCorners(start, y + padding, start + size, y + h - padding);
    } else {
        innerSquare.setCorners(x + padding, y + padding, x + padding + size, y + h - padding);
    }
}