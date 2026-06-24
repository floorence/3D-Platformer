#ifndef SPHERE_H
#define SPHERE_H

#include<glad/glad.h>
#include<vector>
#include<Mesh.h>

class Sphere {
public:
    Sphere(Texture* diffuse, Texture* specular, glm::vec3 position, int radius);

    void registerLightSource(glm::vec4 lightColor, glm::vec3 lightPos);
    void draw(Camera& camera);
private:
    const int NUM_STACKS = 16;
    const int NUM_SECTORS = 32;

    Shader shader;
    Mesh mesh;

    std::vector<Vertex> generateVertices(int radius, int stacks, int sectors);
    std::vector<GLuint> generateIndices(int stacks, int sectors);
    void configureShader(glm::mat4 model);
};

#endif