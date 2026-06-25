#ifndef SPHERE_H
#define SPHERE_H

#include"Shape3D.h"

class Sphere: public Shape3D {
public:
    Sphere(Texture* diffuse, Texture* specular, glm::vec3 position, int radius, int numStacks = 16, int numSectors = 32);
private:
    int numStacks;
    int numSectors;

    std::vector<Vertex> generateVertices(int radius, int stacks, int sectors);
    std::vector<GLuint> generateIndices(int stacks, int sectors);
};

#endif