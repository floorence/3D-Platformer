#include"Stepper.h"
#include "util/Log.h"
#include <string>

bool Stepper::dispatchClick(float x, float y) {
    return decButton.dispatchClick(x, y) || incButton.dispatchClick(x, y);
}

int Stepper::getCount() {
    return count;
}

void Stepper::onBoundsChanged() {
    if (w < 0) {
        w = 3 * h;
    } else if (h < 0) {
        h = w / 3;
    }

    decButton.setBounds(x, y, h, h);
    decButton.setText("-");
    decButton.setOnClick([this]() {
        if (count > min) {
            count--;
            onCountChanged();
        }
    });

    incButton.setBounds(x + w - h, y, h, h);
    incButton.setText("+");
    incButton.setOnClick([this]() {
        if (count < max) {
            count++;
            onCountChanged();
        }
    });

    int fontSize = h * 2 / 3;
    countText.setFontSize(fontSize);
    countText.setCorners(x + h, y, x + w - h, y + h);
}

void Stepper::onCountChanged() {
    countText.setText(std::to_string(count));
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
    onCountChanged(); // technically we dont know that the count changed...
}

void Stepper::setColors(glm::vec3 buttonsColor, glm::vec3 textColor) {
    decButton.setBackgroundColor(buttonsColor);
    incButton.setBackgroundColor(buttonsColor);
    countText.textColor = textColor;
}

void Stepper::draw(Shader& shader, Shader& fontShader) {
    decButton.draw(shader, fontShader);
    countText.draw(fontShader);
    incButton.draw(shader, fontShader);
}