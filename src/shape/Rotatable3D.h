#ifndef ROTATABLE_H
#define ROTATABLE_H

#include "shape/Drawable3D.h"
#include <glm/ext/matrix_float4x4.hpp>

class Rotatable3D: public Drawable3D {
public:
    Rotatable3D() = default;
    Rotatable3D(glm::vec3 position);

    virtual ~Rotatable3D() = default;
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
protected:
    float rotationX, rotationY, rotationZ;
    glm::mat4 rotation = glm::mat4(1.0f);
    /**
     * call this to re-calculate rotation matrix
     */
    void invalidateRotation();
};

#endif