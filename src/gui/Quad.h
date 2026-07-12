#ifndef QUAD_H
#define QUAD_H

#include"texture/Texture.h"
#include"shader/Shader.h"

enum class CoordinateSystem2D {
    CENTER, // 0,0 is at the center (standard coordinate system)
    TOP_LEFT // 0,0 is at the top left and w,h is at the bottom right
};

class Quad {
public:
    Quad(Texture* texture);

    void draw(Shader& shader, CoordinateSystem2D coordSystem, float x, float y, float w, float h);
private:
    Texture* tex = nullptr;
};

#endif