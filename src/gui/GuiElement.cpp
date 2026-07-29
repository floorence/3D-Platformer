#include"GuiElement.h"

GuiElement::GuiElement(float x, float y, float w, float h, bool corners): Rect(x, y, w, h, corners) {}

void GuiElement::setOnClick(std::function<void()> callback) {
    onClickCallback = callback;
}

bool GuiElement::dispatchClick(float x, float y) {
    if (x > this->x && x < this->x + w && y > this->y && y < this->y + h) {
        if (onClickCallback) onClickCallback(); 
        return true;
    }
    return false;
}
