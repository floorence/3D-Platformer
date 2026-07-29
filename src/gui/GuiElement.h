#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include "gui/Clickable.h"
#include "gui/Rect.h"
#include <functional>

class GuiElement: public Rect, public Clickable {
public:
    GuiElement() = default;
    GuiElement(float x, float y, float w, float h, bool corners = false);
    virtual ~GuiElement() = default;

    void setOnClick(std::function<void()> callback);
    bool dispatchClick(float x, float y) override;
private:
    std::function<void()> onClickCallback;
};

#endif