#ifndef QUAD_H
#define QUAD_H

#include"texture/Texture.h"
#include"shader/Shader.h"

class Quad {
public:
    Quad() = default;
    Quad(Texture* texture);

    void setTexture(Texture* texture);
    /**
     * @param x, y top left of quad
     * @param w, h width and height of quad
     * @param reversedYAxis true if y increases downward
     */
    void draw(Shader& shader, float x, float y, float w, float h, bool reversedYAxis);
    /**
     * @param xu, yu top left of quad
     * @param xv, yv bottom right of quad
     */
    void draw(Shader& shader, float xu, float yu, float xv, float yv);
private:
    Texture* tex = nullptr;
    // TODO: refactor to store mesh isntead of textures
};

#endif