#include "Stepper.h"
#include "texture/FontTexture.h"
#include "util/Globals.h"
#include "util/Log.h"
#include <algorithm>
#include <string>

Stepper::Stepper() {
    decButton.setText("-");
    incButton.setText("+");

    decButton.setOnClick([this]() {
        // Log::log("Stepper", "decButton onClick");
        if (count - stepAmount >= min) {
            setData(count - stepAmount);
        }
    });
    incButton.setOnClick([this]() {
        // Log::log("Stepper", "incButton onClick");
        if (count + stepAmount <= max) {
            setData(count + stepAmount);
        }
    });
}

bool Stepper::dispatchMouseEvent(float x, float y, MouseEvent event) {
    return decButton.dispatchMouseEvent(x, y, event) || incButton.dispatchMouseEvent(x, y, event);
}

int Stepper::getData() {
    return count;
}

void Stepper::onBoundsChanged() {
    decButton.setBounds(x, y, h, h);
    incButton.setBounds(x + w - h, y, h, h);

    int fontSize = h * 2 / 3;
    countText.setFontSize(fontSize);
    countText.centerVertically(x + h, w - h - h, -1, y, y + h);
}

float Stepper::getUnboundWidth(float h) {
    std::pair<float, float> textSize = Globals::Font->getSize(std::to_string(max), h * 2 / 3);
    return (h + TEXT_TO_BUTTONS_MARGIN) * 2 + textSize.first;
}

float Stepper::getUnboundHeight(float w) {
    return w / 4;
}

void Stepper::setData(int data) {
    this->count = data;
    countText.setText(std::to_string(count) + units);
}

void Stepper::setStepAmount(int amount) {
    stepAmount = amount;
}

void Stepper::setMinMax(int minCount, int maxCount) {
    min = minCount;
    max = maxCount;
    setData(std::clamp(count, minCount, maxCount));
}

void Stepper::setUnits(std::string units) {
    this->units = units;
    countText.setText(std::to_string(count) + units);
}

void Stepper::setColors(glm::vec3 buttonsColor, glm::vec3 textColor) {
    decButton.setBackgroundColor(buttonsColor);
    incButton.setBackgroundColor(buttonsColor);
    countText.textColor = textColor;
}

void Stepper::draw() {
    decButton.draw();
    countText.draw();
    incButton.draw();
}