#include"Object3D.h"
#include "shape/Drawable3D.h"

Object3D::Object3D(glm::vec3 position) {
    setPosition(position);
}

glm::vec3 Object3D::getPosition() {
    return position;
}

void Object3D::setPosition(glm::vec3 position) {
    this->position = position;
    invalidateModel();
}

void Object3D::setRotation(float rotationX, float rotationY, float rotationZ) {
    rotation = glm::mat4(1.0f);

    if (rotationX) rotation = glm::rotate(rotation, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotationY) rotation = glm::rotate(rotation, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotationZ) rotation = glm::rotate(rotation, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
    
    invalidateModel();
}

void Object3D::setRotation(float angle, glm::vec3 axis) {
    axis = glm::normalize(axis);
    setRotation(angle * axis.x, angle * axis.y, angle * axis.z);
}

void Object3D::setScale(float scale) {
    setScale(glm::vec3(scale));
}

void Object3D::setScale(glm::vec3 scale) {
    this->scale = scale;
    invalidateModel();
}

void Object3D::invalidateModel() {
    // right to left: scale -> rotate -> translate since matrix multiplication is weird like that
    model = glm::translate(glm::mat4(1.0f), position);
    model *= rotation;
    model = glm::scale(model, scale);
}
