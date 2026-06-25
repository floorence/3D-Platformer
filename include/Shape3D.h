#ifndef SHAPE_3D_CLASS
#define SHAPE_3D_CLASS

#include"Mesh.h"

class Shape3D {
public:
    /**
     * @param rotationX rotation around x axis in degrees
     * @param rotationY rotation around y axis in degrees
     * @param rotationZ rotation around z axis in degrees
     */
    Shape3D(Texture* diffuse, Texture* specular, glm::vec3 position, float rotationX = 0.0f, float rotationY = 0.0f, float rotationZ = 0.0f);

    void registerLightSource(glm::vec4 lightColor, glm::vec3 lightPos);
    void draw(Camera& camera);
protected:
    Shader shader;
    Mesh mesh;

    void configureShader(glm::mat4 model);
};

#endif