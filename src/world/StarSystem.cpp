#include "StarSystem.h"
#include "world/World.h"

StarSystem::StarSystem(uint localSeed, Region region)
    : localGen(localSeed),
      star(generateStarPosition(region), World::REGION_SIZE / 20)
{
    // TODO: star mass, size, colour, luminosity
    generatePlanets();
}

glm::vec3 StarSystem::generateStarPosition(Region region) {
    int startX = region.x * World::REGION_SIZE;
    int startY = region.y * World::REGION_SIZE;
    int startZ = region.z * World::REGION_SIZE;
    
    int offsetX = random(0, World::REGION_SIZE - 1);
    int offsetY = random(0, World::REGION_SIZE - 1);
    int offsetZ = random(0, World::REGION_SIZE - 1);

    return {startX + offsetX, startY + offsetY, startZ + offsetZ};
}

void StarSystem::generatePlanets() {
    int numPlanets = random(0, 10);
    int currDist = 0;
    for (int i = 0; i < numPlanets; i++) {
        // int distance = 
    }
}

int StarSystem::random(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(localGen);
}