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
    auto [width, height] = processDimensions(w, h);
    this->x = x;
    this->y = y;
    this->w = disableDimensionsProcessing ? w : width;
    this->h = disableDimensionsProcessing ? h : height;
    onBoundsChanged();
}

void Rect::setBoundsEnd(float x, float y, float w, float h) {
    auto [width, height] = processDimensions(w, h);
    setBounds(x - width, y, width, height);
}

void Rect::setCorners(float xu, float yu, float xv, float yv) {
    setBounds(xu, yu, xv - xu, yv - yu);
}

void Rect::centerHorizontally(float y, float w, float h, float start, float end) {
    auto [width, height] = processDimensions(w, h);
    float middle = start + (end - start) / 2;
    setBounds(middle - width / 2, y, width, height);
}

void Rect::centerVertically(float x, float w, float h, float start, float end) {
    auto [width, height] = processDimensions(w, h);
    float middle = start + (end - start) / 2;
    setBounds(x, middle - height / 2, width, height);
}

void Rect::center(float w, float h, float startX, float endX, float startY, float endY) {
    auto [width, height] = processDimensions(w, h);
    float middleX = startX + (endX - startX) / 2;
    float middleY = startY + (endY - startY) / 2;

    setBounds(middleX - width / 2, middleY - height / 2, width, height);
}

std::pair<float, float> Rect::processDimensions(float w, float h) {
    if (w < 0 && h < 0) {
        Log::err("Rect", fmt::format("processDimensions({}, {}), only one of width or height may be unbound! setting both width and height to 0.", w, h));
        return {0, 0};
    }
    return {
        (w < 0) ? getUnboundWidth(h) : w,
        (h < 0) ? getUnboundHeight(w) : h
    };
}
