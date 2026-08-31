#ifndef CLICKABLE_H
#define CLICKABLE_H

enum class MouseEvent {
    Down, Up, Hover
};

class Clickable {
public:
    virtual ~Clickable() = default;
    /** @return whether or not click was registered */
    virtual bool dispatchMouseEvent(float x, float y, MouseEvent event) = 0;
};

#endif