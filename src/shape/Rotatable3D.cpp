#include"Rotatable3D.h"
#include "shape/Drawable3D.h"

Rotatable3D::Rotatable3D(glm::vec3 position): Drawable3D(position) {}

void Rotatable3D::setRotation(float rotationX, float rotationY, float rotationZ) {
    this->rotationX = rotationX;
    this->rotationY = rotationY;
    this->rotationZ = rotationZ;

    invalidateRotation();
}

void Rotatable3D::setRotation(float angle, glm::vec3 axis) {
    axis = glm::normalize(axis);
    setRotation(angle * axis.x, angle * axis.y, angle * axis.z);
}

void Rotatable3D::invalidateRotation() {
    rotation = glm::mat4(1.0f);

    if (rotationX) rotation = glm::rotate(rotation, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotationY) rotation = glm::rotate(rotation, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotationZ) rotation = glm::rotate(rotation, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

    invalidateModel(); // reset model; apply translation first and rotation second since matrix multiplication in glm works from right to left
     
    if (rotationX) model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotationY) model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotationZ) model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
}