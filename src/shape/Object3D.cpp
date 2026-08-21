#include"Object3D.h"
#include "shape/Drawable3D.h"
#include "util/Log.h"

Object3D::Object3D(glm::vec3 position) {
    setPosition(position);
}

std::string getDebugString();

glm::vec3 Object3D::getPosition() {
    return position;
}

void Object3D::setPosition(glm::vec3 position) {
    this->position = position;
    invalidateModel();
}

void Object3D::setRotation(float rotationX, float rotationY, float rotationZ) {
    rotation = glm::mat4(1.0f);
    // Log::log(TAG, fmt::format("rotationX: {}, rotationY: {}, rotationZ: {}", rotationX, rotationY, rotationZ));
    float rotX = rotationX + defaultRotation.x;
    float rotY = rotationY + defaultRotation.y;
    float rotZ = rotationZ + defaultRotation.z;

    if (rotX) rotation = glm::rotate(rotation, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotY) rotation = glm::rotate(rotation, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotZ) rotation = glm::rotate(rotation, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
    
    invalidateModel();
}

void Object3D::setRotation(float angle, glm::vec3 axis) {
    axis = glm::normalize(axis);
    setRotation(angle * axis.x, angle * axis.y, angle * axis.z);
}

void Object3D::setDefaultRotation(float rotationX, float rotationY, float rotationZ) {
    defaultRotation = glm::vec3(rotationX, rotationY, rotationZ);
    setRotation(0.0f, 0.0f, 0.0f); // apply default rotation
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
