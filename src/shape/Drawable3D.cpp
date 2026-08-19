#include"Drawable3D.h"

Drawable3D::Drawable3D(glm::vec3 position) {
    setPosition(position);
}

glm::vec3 Drawable3D::getPosition() {
    return position;
}

void Drawable3D::setPosition(glm::vec3 position) {
    this->position = position;
    invalidateModel();
}

void Drawable3D::invalidateModel() {
    model = glm::translate(glm::mat4(1.0f), position);
}