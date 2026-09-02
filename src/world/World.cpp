#include "World.h"
#include "util/Log.h"
#include <random>

World::World(uint seed, Player* player) 
    : seed(seed), player(player) 
{
    loadStartingRegions();
}

void World::loadStartingRegions() {
    Region playerCube = {
        static_cast<int>(player->position.x / 100), 
        static_cast<int>(player->position.y / 100), 
        static_cast<int>(player->position.z / 100), 
    };
    int r = 5;
    for (int x = playerCube.x - r; x <= playerCube.x + r; x++) {
        for (int y = playerCube.y - r; y <= playerCube.y + r; y++) {
            for (int z = playerCube.z - r; z <= playerCube.z + r; z++) {
                loadIfNotLoaded({x, y, z});
            }
        }
    }
}

void World::loadIfNotLoaded(Region region) {
    bool hasLoaded = std::binary_search(loaded.begin(), loaded.end(), region);
    if (hasLoaded) return;
    
    if (actuallyHasStarSystem(region)) {
        Log::log("World", fmt::format("star system at region {}, {}, {}", region.x, region.y, region.z));
    }
}

int World::getRegionRandom(Region region, int min, int max) {
    uint localSeed = seed ^ (region.x * 37) ^ (region.y * 67) ^ (region.z * 73); // prime numbers
    std::mt19937 localGen(localSeed);
    std::uniform_int_distribution<int> dist(min, max);
    return dist(localGen);
}

uint64_t World::splitmix64(uint64_t x) {
    x += 0x9e3779b97f4a7c15ULL;

    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);

    return x;
}

uint64_t World::hashRegion(Region region, uint64_t salt) {
    uint64_t h = seed;

    h = splitmix64(h ^ static_cast<uint64_t>(region.x));
    h = splitmix64(h ^ static_cast<uint64_t>(region.y));
    h = splitmix64(h ^ static_cast<uint64_t>(region.z));
    h = splitmix64(h ^ salt);

    return h;
}

double World::hash01(Region region, uint64_t salt) {
    uint64_t h = hashRegion(region, salt);

    return static_cast<double>(h) / static_cast<double>(UINT64_MAX);
}

bool World::potentiallyHasStarSystem(Region region) {
    return hash01(region, 0) < 0.5;
}

double World::getPriority(Region region) {
    return hash01(region, 1);
}

bool World::actuallyHasStarSystem(Region region) {
    int potentially = potentiallyHasStarSystem(region);
    if (!potentially) return false;

    double myPriority = getPriority(region);

    // star systems can't be in regions right next to each other including diagonal regions 
    // to avoid an infinite loop, a region yields to its neighbours if the neighbour has a higher priority.
    for (int x = region.x - 1; x <= region.x + 1; x++) {
        for (int y = region.y - 1; y <= region.y + 1; y++) {
            for (int z = region.z - 1; z <= region.z + 1; z++) {
                if (region.x == x && region.y == y && region.z == z) continue; // skip myself
                
                Region neighbor = {x, y, z};
                if (!potentiallyHasStarSystem(neighbor)) continue;
                if (getPriority(neighbor) > myPriority) return false;
            }
        }
    }
    
    return true;
}