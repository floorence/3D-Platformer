#include"Rect.h"
#include "util/Log.h"
#include <fmt/format.h>

Rect::Rect(float x, float y, float w, float h, bool corners) {
    if (corners) {
        setCorners(x, y, w, h);
    } else {
        setBounds(x, y, w, h);
    }
}

void Rect::setBounds(float x, float y, float w, float h) {
    if (w < 0 && h < 0) {
        Log::err("Rect", fmt::format("setBounds({}, {}, {}, {}), only one of width or height may be unbound! bounds not set.", x, y, w, h));
        return;
    }
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    onBoundsChanged();
}

void Rect::setCorners(float xu, float yu, float xv, float yv) {
    setBounds(xu, yu, xv - xu, yv - yu);
}

void Rect::centerHorizontally(float y, float w, float h, float start, float end) {
    float middle = start + (end - start) / 2;
    setBounds(middle - w / 2, y, w, h);
}

void Rect::centerVertically(float x, float w, float h, float start, float end) {
    float middle = start + (end - start) / 2;
    setBounds(x, middle - h / 2, w, h);
}

void Rect::center(float w, float h, float startX, float endX, float startY, float endY) {
    float middleX = startX + (endX - startX) / 2;
    float middleY = startY + (endY - startY) / 2;

    setBounds(middleX - w / 2, middleY - h / 2, w, h);
}
