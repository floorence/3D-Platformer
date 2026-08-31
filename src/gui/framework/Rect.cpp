#include "Rect.h"
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
    specX = x; specY = y;
    alignment = Alignment::TopLeft;
    setBoundsInternal(x, y, width, height);
}

void Rect::setBoundsEnd(float x, float y, float w, float h) {
    auto [width, height] = processDimensions(w, h);
    specX = x; specY = y;
    alignment = Alignment::TopRight;
    setBoundsInternal(x - width, y, width, height);
}

void Rect::setCorners(float xu, float yu, float xv, float yv) {
    setBounds(xu, yu, xv - xu, yv - yu);
}

void Rect::centerHorizontally(float y, float w, float h, float start, float end) {
    auto [width, height] = processDimensions(w, h);
    float middle = start + (end - start) / 2;
    specY = y; startX = start; endX = end;
    alignment = Alignment::CenterHorizontal;
    setBoundsInternal(middle - width / 2, y, width, height);
}

void Rect::centerVertically(float x, float w, float h, float start, float end) {
    auto [width, height] = processDimensions(w, h);
    float middle = start + (end - start) / 2;
    specX = x; startY = start; endY = end;
    alignment = Alignment::CenterVertical;
    setBoundsInternal(x, middle - height / 2, width, height);
}

void Rect::center(float w, float h, float startX, float endX, float startY, float endY) {
    auto [width, height] = processDimensions(w, h);
    float middleX = startX + (endX - startX) / 2;
    float middleY = startY + (endY - startY) / 2;

    this->startX = startX; this->endX = endX; this->startY = startY; this->endY = endY;
    alignment = Alignment::Center;
    setBoundsInternal(middleX - width / 2, middleY - height / 2, width, height);
}

void Rect::invalidateBounds() {
    switch (alignment) {
        case Alignment::TopLeft: 
            setBounds(specX, specY, specW, specH);
            break;
        case Alignment::TopRight:
            setBoundsEnd(specX, specY, specW, specH);
            break;
        case Alignment::CenterHorizontal:
            centerHorizontally(specY, specW, specH, startX, endX);
            break;
        case Alignment::CenterVertical:
            centerVertically(specX, specW, specH, startY, endY);
            break;
        case Alignment::Center:
            center(specW, specH, startX, endX, startY, endY);
            break;
    }
}

void Rect::setBoundsInternal(float x, float y, float w, float h) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    onBoundsChanged();
}

std::pair<float, float> Rect::processDimensions(float w, float h) {
    specW = w;
    specH = h;
    if (disableDimensionsProcessing) return {w, h};
    if (!enableUnboundWidthAndHeight && w < 0 && h < 0) {
        Log::err("Rect", fmt::format("processDimensions({}, {}), only one of width or height may be unbound! setting both width and height to 0.", w, h));
        return {0, 0};
    }
    
    return {
        (w < 0) ? getUnboundWidth(h) : w,
        (h < 0) ? getUnboundHeight(w) : h
    };
}
