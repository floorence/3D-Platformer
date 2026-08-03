#ifndef CLICKABLE_H
#define CLICKABLE_H

class Clickable {
public:
    virtual ~Clickable() = default;
    /** @return whether or not click was registered */
    virtual bool dispatchClick(float x, float y) = 0;
};

#endif