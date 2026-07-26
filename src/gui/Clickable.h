#ifndef CLICKABLE_H
#define CLICKABLE_H

class Clickable {
public:
    virtual ~Clickable() = default;
    virtual bool dispatchClick(float x, float y) = 0;
};

#endif