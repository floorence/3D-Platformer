#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "shape/Drawable3D.h"
#include <glm/ext/matrix_float4x4.hpp>

class Object3D: public Drawable3D {
public:
    Object3D() = default;
    Object3D(glm::vec3 position);

    virtual ~Object3D() = default;

    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);
    /**
     * @param rotationX rotation around x axis in degrees
     * @param rotationY rotation around y axis in degrees
     * @param rotationZ rotation around z axis in degrees
     */
    void setRotation(float rotationX, float rotationY, float rotationZ);
    /**
     * @param angle angle of rotation in degrees
     * @param axis axis of rotation
     */
    void setRotation(float angle, glm::vec3 axis);
    void setScale(float scale);
    void setScale(glm::vec3 scale);
protected:
    glm::mat4 model = glm::mat4(1.0f); // shader uses model to place vertices around correct location in the world
    glm::vec3 position = glm::vec3(0.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    /**
     * resets model with current position, call this after updating position.
     * call this before applying rotation to model since matrix multiplication in glm works from right to left
     */
    virtual void invalidateModel();
};

#endif