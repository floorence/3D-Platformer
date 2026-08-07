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
    bool dispatchMouseEvent(float x, float y, MouseEvent event) override;

    virtual void onHover() {}; // default behaviour: do nothing
    virtual void onHoverOff() {}; // default behaviour: do nothing
private:
    std::function<void()> onClickCallback;
    bool previousFrameHover = false;
};

#endif