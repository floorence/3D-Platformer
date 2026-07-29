#include"Stepper.h"
#include "util/Log.h"
#include <string>

Stepper::Stepper(float xu, float yu, float xv, float yv) {
    setCorners(xu, yu, xv, yv);
}

bool Stepper::dispatchClick(float x, float y) {
    return decButton.dispatchClick(x, y) || incButton.dispatchClick(x, y);
}

int Stepper::getCount() {
    return count;
}

void Stepper::onBoundsChanged() {
    decButton.setCorners(x, y, x + h, y + h);
    decButton.text = "-";
    decButton.setOnClick([this]() {
        if (count > min) count--;
    });

    incButton.setCorners(x + w - h, y, x + w, y + h);
    incButton.text = "+";
    incButton.setOnClick([this]() {
        if (count < max) count++;
    });
}

void Stepper::setCountAndMinMax(int count, int minCount, int maxCount) {
    min = minCount;
    max = maxCount;
    if (count < minCount || count > maxCount) {
        Log::err("Stepper", "setCountAndBounds() given count is outside of bounds! defaulting to min.");
        this->count = minCount;
    } else {
        this->count = count;
    }
}

void Stepper::setColors(glm::vec3 buttonsColor, glm::vec3 textColor) {
    decButton.setBackgroundColor(buttonsColor);
    incButton.setBackgroundColor(buttonsColor);
    this->textColor = textColor;
}

void Stepper::draw(Shader& shader, TextRenderer& textRenderer) {
    decButton.draw(shader, textRenderer);
    textRenderer.drawText(std::to_string(count), x + h + 2.0f, y + 2.0f, 30, 16); // TODO
    incButton.draw(shader, textRenderer);
}