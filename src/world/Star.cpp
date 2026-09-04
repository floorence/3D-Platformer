#include "Star.h"
#include "util/Utils.h"

Star::Star(glm::vec3 position, int radius)
    : Sphere(position, radius, 16, 32, true) 
{
    this->Mass::position = position;
}

glm::vec3 Star::getNorth() {
    return north;
}

void Star::setNorth(glm::vec3 north) {
    this->north = north;
    northLine.setCoordinates(
        this->Object3D::position,
        this->Object3D::position + Utils::setVectorLength(north, radius + 4)
    );
}

void Star::draw(Camera& camera) {
    Shape3D::draw(camera);
    northLine.draw(camera);
}