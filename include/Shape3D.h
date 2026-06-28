#ifndef SHAPE_3D_CLASS
#define SHAPE_3D_CLASS

#include"Mesh.h"

class Shape3D {
public:
    Shape3D(glm::vec3 position, bool isLightSource);
    Shape3D(Texture* diffuse, Texture* specular, glm::vec3 position, bool isLightSource);
    virtual ~Shape3D() = default;

    /**
     * @param rotationX rotation around x axis in degrees
     * @param rotationY rotation around y axis in degrees
     * @param rotationZ rotation around z axis in degrees
     */
    void setRotation(float rotationX, float rotationY, float rotationZ);
    void setColor(glm::vec4 color, float intensity);
    void registerLightSource(glm::vec4 lightColor, glm::vec3 lightPos);
    void draw(Camera& camera);
protected:
    glm::vec3 position;
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
    bool isLightSource = false;
    glm::mat4 model = glm::mat4(1.0f);

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    Shader shader;
    Mesh mesh;
    void configureShader(glm::mat4 model);
    virtual std::vector<Vertex> generateVertices() = 0;
    virtual std::vector<GLuint> generateIndices() = 0;
};

#endif