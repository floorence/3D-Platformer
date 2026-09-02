#pragma once

#include "3d/shape/Sphere.h"
#include "mass/Mass.h"

class Star: public Mass, public Sphere {
public:
    Star(glm::vec3 position, int radius);

};