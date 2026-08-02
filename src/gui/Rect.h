#ifndef RECT_H
#define RECT_H

class Rect {
public:
    // TODO: add way to change only one at a time

    Rect() = default;
    Rect(float x, float y, float w, float h, bool corners = false);
    virtual ~Rect() = default;

    /**
     * @param x, y top left of quad
     * @param w, h width and height of quad; passing negative for either value may be interpreted
     *             by subclasses as an "unbound" value and an actual value will be set accordingly
     */
    void setBounds(float x, float y, float w, float h);
    /**
     * @param xu, yu top left of quad
     * @param xv, yv bottom right of quad
     */
    void setCorners(float xu, float yu, float xv, float yv);

    void centerHorizontally(float y, float w, float h, float start, float end);
    void centerVertically(float x, float w, float h, float start, float end);
    void center(float w, float h, float startX, float endX, float startY, float endY);
protected:
    float x, y, w, h;
private:
    virtual void onBoundsChanged() {};
};

#endif