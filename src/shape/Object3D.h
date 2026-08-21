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
     * @param angle angle of rotation in degrees, counterclockwise around a positive axis
     * @param axis axis of rotation
     */
    void setRotation(float angle, glm::vec3 axis);
    /** @brief set the rotation that should always be applied on top of setRotation() */
    void setDefaultRotation(float rotationX, float rotationY, float rotationZ);
    /**
     * @param scale scale to apply to x, y, and z axes
     */
    void setScale(float scale);
    /**
     * @param scale scale vector whose components represent scale in x, y, and z axes
     */
    void setScale(glm::vec3 scale);
protected:
    glm::mat4 model = glm::mat4(1.0f); // shader uses model to place vertices around correct location in the world
    glm::vec3 position = glm::vec3(0.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::vec3 defaultRotation = glm::vec3(0.0f);

    const std::string TAG = "Object3D";

    /**
     * resets model with current position, rotation, and scale.
     */
    virtual void invalidateModel();
};

#endif