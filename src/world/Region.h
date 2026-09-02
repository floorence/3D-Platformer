#pragma once

#include <tuple>

struct Region {
    int x, y, z; // coordinate of the corner that is in the negative x, y, and z direction relative to the rest of the region, divided by 100.

    bool operator<(const Region& other) const {
        return std::tie(x, y, z) < std::tie(other.x, other.y, other.z);
    }
    
    bool operator==(const Region& other) const {
        return std::tie(x, y, z) == std::tie(other.x, other.y, other.z);
    }
};