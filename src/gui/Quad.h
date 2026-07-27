#ifndef QUAD_H
#define QUAD_H

#include "mesh/Mesh.h"
#include"texture/Texture.h"
#include"shader/Shader.h"

class Quad {
public:
    bool useColorInsteadOfTexture = false;
    glm::vec3 color;

    Quad() = default;
    Quad(Texture* texture);

    virtual ~Quad() = default;

    void setTexture(Texture* texture);
    void setTextures(std::vector<Texture*> textures);
    /**
     * @brief another way to set bounds using width and height instead of top left and bottom right corner.
     *        internally calls setBounds(xu, yu, xv, yv)
     * @param x, y top left of quad
     * @param w, h width and height of quad
     * @param reversedYAxis true if y increases downward
     */
    void setBounds(float x, float y, float w, float h, bool reversedYAxis);
    /**
     * @param xu, yu top left of quad
     * @param xv, yv bottom right of quad
     */
    virtual void setBounds(float xu, float yu, float xv, float yv);
    
    virtual void draw(Shader& shader);
protected:
    float xu, yu, xv, yv;
private:
    Mesh mesh;
};

#endif