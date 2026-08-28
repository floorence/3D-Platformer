#include"CycleButton.h"

CycleButton::CycleButton() {
    button.setOnClick([this]() {
        if (options.size() != 0) {
            setData((option + 1) % options.size());
        }
    });
}

bool CycleButton::dispatchMouseEvent(float x, float y, MouseEvent event) {
    return button.dispatchMouseEvent(x, y, event);
}

void CycleButton::onBoundsChanged() {
    button.setBounds(x, y, w, h);
}

float CycleButton::getUnboundWidth(float h) {
    return button.getUnboundWidth(h);
}

float CycleButton::getUnboundHeight(float w) {
    return button.getUnboundHeight(w);
}

int CycleButton::getData() {
    return option;
}

void CycleButton::setData(int data) {
    option = data;
    button.setText(options[option]);
    invalidateBounds();
}

void CycleButton::setOptions(std::vector<std::string> options) {
    this->options = options;
    setData(0);
}

void CycleButton::setBackgroundColor(glm::vec3 color) {
    button.setBackgroundColor(color);
}

void CycleButton::draw() {
    button.draw();
}
