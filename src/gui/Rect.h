#ifndef RECT_H
#define RECT_H

#include <string>
class Rect {
public:
    // set to true if dimensions are expected to be negative for some reason. ONLY FOR LIGHTCONTROLLER QUADS!
    bool disableDimensionsProcessing = false;
    // TODO: add way to change only one at a time

    Rect() = default;
    Rect(float x, float y, float w, float h, bool corners = false);
    virtual ~Rect() = default;

    /**
     * @param x, y top left of rect
     * @param w, h width and height of rect; passing negative for either value will be interpreted
     *             as an "unbound" value and an actual value will be set accordingly
     */
    void setBounds(float x, float y, float w, float h);
    /** @brief same as setBounds but x value is the right of rect. */
    void setBoundsEnd(float x, float y, float w, float h);
    /**
     * @param xu, yu top left of rect
     * @param xv, yv bottom right of rect
     */
    void setCorners(float xu, float yu, float xv, float yv);

    void centerHorizontally(float y, float w, float h, float start, float end);
    void centerVertically(float x, float w, float h, float start, float end);
    void center(float w, float h, float startX, float endX, float startY, float endY);
protected:
    float x, y, w, h;
private:
    std::string TAG = "Rect";

    virtual void onBoundsChanged() {};
    virtual float getUnboundWidth(float h) { return h; };
    virtual float getUnboundHeight(float w) { return w; };
    std::pair<float, float> processDimensions(float w, float h);
};

#endif