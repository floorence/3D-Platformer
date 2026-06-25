#ifndef RECTANGULAR_PRISM_H
#define RECTANGULAR_PRISM_H

#include "Shape3D.h"

enum class Facing {
    negX, posX, negY, posY, negZ, posZ
};

class RectangularPrism: public Shape3D {
public:
    /**
     * @param width size in x axis
     * @param length size in z axis
     * @param height size in y axis
     */ 
    RectangularPrism(Texture* diffuse, Texture* specular, glm::vec3 position, int width, int length, int height);
private:
    std::vector<Vertex> generateVertices(int width, int length, int height);
    std::vector<GLuint> generateIndices();
    glm::vec2 deriveTexCoord(glm::vec3 vertex, Facing dir);
};

#endif