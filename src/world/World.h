#pragma once

#include "mass/Player.h"
#include "world/Region.h"
#include "world/StarSystem.h"
#include <glm/ext/vector_float3.hpp>
#include <vector>

/**
 * * split the world into 100x100x100 regions
 * * regions will be tracked by the coordinate of the corner that is in the negative x, y, and z direction relative to the rest of the region, divided by 100.
 * * a region can have 1 star system or have nothing in it
 * * star systems can't be in regions right next to each other or diagonal to each other
 * * in valid regions, a star system can spawn 50% of the time
 * * only regions that have been loaded by the player will be considered. a player loads a region if they enter a region that is
 *   2 regions away from it, including diagonals. the region that was entered will notify all regions that are 2 regions away
 *   and tell them to load if not already. this number should be configurable
 * * if player is at 0,0,0 they will be considered inside the region tracked by 0,0,0. same logic applies to other regions 
 * * also only star systems within the loading distance of the player would simulate gravity and stuff.
 * * once a region has decided that it'll have a star system, the StarSystem class does the work of generating its planets and stuff
 */
class World {
public:
    World(uint seed, Player* player);

private:
    uint seed;
    Player* player;

    std::vector<StarSystem> starSystems;
    std::vector<Region> loaded;

    void loadStartingRegions();
    void loadIfNotLoaded(Region region);

    int getRegionRandom(Region region, int min, int max);
    bool potentiallyHasStarSystem(Region region);
    double getPriority(Region region);
    bool actuallyHasStarSystem(Region region);

    uint64_t splitmix64(uint64_t x);
    uint64_t hashRegion(Region region, uint64_t salt);
    double hash01(Region region, uint64_t salt); // [0, 1) 
};