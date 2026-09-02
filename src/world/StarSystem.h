#pragma once

#include "world/Planet.h"
#include "world/Region.h"
#include "world/Star.h"
#include <glm/ext/vector_float3.hpp>
#include <random>

class StarSystem {
public:
    StarSystem(uint localSeed, Region region);

    void update(float deltaTime);
private:
    std::mt19937 localGen;

    Star star;
    std::vector<Planet> planets;

    glm::vec3 generateStarPosition(Region region);
    glm::vec3 generateStarNorth();
    void generatePlanets();
    int random(int min, int max);
};