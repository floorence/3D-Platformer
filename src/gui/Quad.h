#ifndef QUAD_H
#define QUAD_H

#include "mesh/Mesh.h"
#include"texture/Texture.h"
#include"shader/Shader.h"

class Quad {
public:
    Quad() = default;
    Quad(Texture* texture);

    void setTexture(Texture* texture);
    void setTextures(std::vector<Texture*> textures);
    /**
     * @param x, y top left of quad
     * @param w, h width and height of quad
     * @param reversedYAxis true if y increases downward
     */
    void setBounds(float x, float y, float w, float h, bool reversedYAxis);
    /**
     * @param xu, yu top left of quad
     * @param xv, yv bottom right of quad
     */
    void setBounds(float xu, float yu, float xv, float yv);
    
    void draw(Shader& shader);
private:
    Mesh mesh;
};

#endif