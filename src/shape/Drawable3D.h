#ifndef DRAWABLE_3D_H
#define DRAWABLE_3D_H

#include "camera/Camera.h"
#include "mesh/VBO.h"
#include "shader/Shader.h"

class Drawable3D {
public:
    Shader* specialShader = nullptr; // optional special shader to use instead of default

    Drawable3D() = default;
    Drawable3D(glm::vec3 position);

    virtual ~Drawable3D() = default;

    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);

    virtual void draw(Camera& camera, Shader& shader) = 0;
    virtual void drawToDepthMap(PointLightCamera& camera, Shader& depthShader) = 0;
protected:
    glm::vec3 position;
    glm::mat4 model = glm::mat4(1.0f); // shader uses model to place vertices around correct location in the world

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    /**
     * resets model with current position, call this after updating position.
     * call this before applying rotation to model since matrix multiplication in glm works from right to left
     */
    virtual void invalidateModel();
};

#endif