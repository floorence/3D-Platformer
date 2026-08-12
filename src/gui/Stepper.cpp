#include"Stepper.h"
#include "util/Log.h"
#include <algorithm>
#include <string>

Stepper::Stepper() {
    decButton.setText("-");
    incButton.setText("+");

    decButton.setOnClick([this]() {
        if (count - stepAmount >= min) {
            setData(count - stepAmount);
        }
    });
    incButton.setOnClick([this]() {
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
    countText.setCorners(x + h, y, x + w - h, y + h);
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