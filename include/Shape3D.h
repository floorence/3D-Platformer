#ifndef SHAPE_3D_CLASS
#define SHAPE_3D_CLASS

#include"Mesh.h"

struct Light {
    glm::vec3 color;
    float range;
    glm::vec3 direction = glm::vec3(0.0f); // 0 for point light, not 0 for spot light
};

class Shape3D {
public:
    glm::vec3 position;
    bool isLightSource = false;
    Light light; // will be ignored if isLightSource = false. TODO this is kinda cringe

    Shape3D(glm::vec3 position, bool isLightSource);
    Shape3D(Texture* diffuse, Texture* specular, glm::vec3 position, bool isLightSource);
    virtual ~Shape3D() = default;

    /**
     * @param rotationX rotation around x axis in degrees
     * @param rotationY rotation around y axis in degrees
     * @param rotationZ rotation around z axis in degrees
     */
    void setRotation(float rotationX, float rotationY, float rotationZ);
    /**
     * @param color if isLightSource, will be the colour of the light. else, will be the tintColor of the shape
     * @param intensity if isLightSource, will be the range of the light. else, will be the intensity of the tint (0 - 1)
     */
    void setColor(glm::vec3 color, float intensity);
    void registerLightSource(int num, glm::vec3 lightColor, glm::vec3 lightPos, float linear, float quadratic);
    void setNumPointLights(int num);
    void draw(Camera& camera);
protected:
    float rotationX, rotationY, rotationZ;
 
    /**
     * call this after updating rotation or any subclass fields used by generateVertices/Indices
     */
    void invalidateShape();
    /**
     * call this after updating position and/or rotation.
     */
    void invalidateModel();
private:
    glm::mat4 model = glm::mat4(1.0f);

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    Shader shader;
    Mesh mesh;
    const std::string TAG = "Shape3D";

    void configureShader(glm::mat4 model);
    virtual std::vector<Vertex> generateVertices() = 0;
    virtual std::vector<GLuint> generateIndices() = 0;
};

#endif