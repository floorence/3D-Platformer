#include"Stepper.h"
#include "util/Log.h"
#include <string>

Stepper::Stepper() {
    countText.setFontSize(16);
}

Stepper::Stepper(float xu, float yu, float xv, float yv): Stepper() {
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
    decButton.setText("-");
    decButton.setOnClick([this]() {
        if (count > min) count--;
    });

    incButton.setCorners(x + w - h, y, x + w, y + h);
    incButton.setText("+");
    incButton.setOnClick([this]() {
        if (count < max) count++;
    });

    countText.setCorners(x + h, y, x + w - h, y + h);
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
    countText.textColor = textColor;
}

void Stepper::draw(Shader& shader, Shader& fontShader) {
    decButton.draw(shader, fontShader);
    countText.setText(std::to_string(count)); // TODO
    countText.draw(fontShader);
    incButton.draw(shader, fontShader);
}