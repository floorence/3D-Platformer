#include"Stepper.h"
#include "util/Log.h"
#include <string>

Stepper::Stepper(float xu, float yu, float xv, float yv) {
    setBounds(xu, yu, xv, yv);
}

bool Stepper::dispatchClick(float x, float y) {
    return decButton.dispatchClick(x, y) || incButton.dispatchClick(x, y);
}

int Stepper::getCount() {
    return count;
}

void Stepper::setBounds(float xu, float yu, float xv, float yv) {
    float buttonSize = yv - yu;

    decButton.useColorInsteadOfTexture = true;
    decButton.setBounds(xu, yu, xu + buttonSize, yv);
    decButton.text = "-";
    decButton.setOnClick([this]() {
        if (count > min) count--;
    });

    incButton.useColorInsteadOfTexture = true;
    incButton.setBounds(xv - buttonSize, yu, xv, yv);
    incButton.text = "+";
    incButton.setOnClick([this]() {
        if (count < max) count++;
    });

    textX = xu + buttonSize + 2.0f;
    textY = yu + 2.0f;
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
    decButton.color = buttonsColor;
    incButton.color = buttonsColor;
    this->textColor = textColor;
}

void Stepper::draw(Shader& shader, TextRenderer& textRenderer) {
    decButton.drawWithText(shader, textRenderer);
    textRenderer.drawText(std::to_string(count), textX, textY, 30, 16); // TODO
    incButton.drawWithText(shader, textRenderer);
}