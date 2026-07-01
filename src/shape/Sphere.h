#ifndef SPHERE_H
#define SPHERE_H

#include"Shape3D.h"

class Sphere: public Shape3D {
public:
    Sphere(Texture* diffuse, Texture* specular, glm::vec3 position, float radius, int stacks = 16, int sectors = 32, bool isLightSource = false);
private:
    float radius;
    int stacks, sectors;

    std::vector<Vertex> generateVertices() override;
    std::vector<GLuint> generateIndices() override;
};

#endif