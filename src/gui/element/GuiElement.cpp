#include "GuiElement.h"
#include "util/Log.h"

GuiElement::GuiElement(float x, float y, float w, float h, bool corners): Rect(x, y, w, h, corners) {}

void GuiElement::setOnClick(std::function<void()> callback) {
    onClickCallback = callback;
}

bool GuiElement::dispatchMouseEvent(float x, float y, MouseEvent event) {
    if (x > this->x && x < this->x + w && y > this->y && y < this->y + h) {
        if (event == MouseEvent::Down) {
            if (onClickCallback) onClickCallback(); 
        } else if (event == MouseEvent::Hover) {
            if (!previousFrameHover) {
                onHover();
                previousFrameHover = true;
            }
        }
        return true;
    } else { // outside my bounds
        if (event == MouseEvent::Hover) {
            if (previousFrameHover) {
                onHoverOff();
                previousFrameHover = false;
            }
        }
    }
    return false;
}
