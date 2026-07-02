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
    RectangularPrism(Texture* diffuse, Texture* specular, glm::vec3 position, 
                     float width, float height, float length, bool isLightSource = false);
    /**
     * @brief texture will be repeated across the surface; set how big you want one repetition of the texture to be. 
     */
    void setTextureSize(float width, float height);
private:
    float width, height, length;
    float texWidth = 1.0f, texHeight = 1.0f;
    const std::string TAG = "RectangularPrism";

    std::vector<Vertex> generateVertices() override;
    std::vector<GLuint> generateIndices() override;
    glm::vec2 deriveTexCoord(glm::vec3 vertex, Facing dir);
};

#endif