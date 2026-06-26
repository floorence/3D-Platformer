#ifndef SHAPE_3D_CLASS
#define SHAPE_3D_CLASS

#include"Mesh.h"

class Shape3D {
public:
    Shape3D(Texture* diffuse, Texture* specular, glm::vec3 position);

    /**
     * @param rotationX rotation around x axis in degrees
     * @param rotationY rotation around y axis in degrees
     * @param rotationZ rotation around z axis in degrees
     */
    void setRotation(float rotationX, float rotationY, float rotationZ);
    void registerLightSource(glm::vec4 lightColor, glm::vec3 lightPos);
    void draw(Camera& camera);
protected:
    glm::vec3 position;
    std::vector<Vertex> vertices;
    glm::mat4 model = glm::mat4(1.0f);

    Shader shader;
    Mesh mesh;

    void configureShader(glm::mat4 model);
};

#endif