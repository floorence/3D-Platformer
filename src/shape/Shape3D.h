#ifndef SHAPE_3D_CLASS
#define SHAPE_3D_CLASS

#include"mesh/Mesh.h"
#include"texture/AssetTexture.h"

class Shape3D {
public:
    bool isLightSource = false;

    glm::vec3 color; // if isLightSource, will be the colour of the light. else, will not be used

    // if !isLightSource, will be the tint colour of the light. else, will not be used
    // higher alpha value will lead to stronger tint
    glm::vec4 tintColor = glm::vec4(0.0f);  
    glm::vec3 direction = glm::vec3(0.0f); // 0 for point light, not 0 for spot light

    Shader* specialShader = nullptr; // optional special shader to use instead of default

    Shape3D() = default;
    Shape3D(glm::vec3 position, bool isLightSource);
    Shape3D(AssetTexture* diffuse, AssetTexture* specular, glm::vec3 position, bool isLightSource);
    virtual ~Shape3D() = default;

    glm::vec3 getPosition();

    void setTextures(AssetTexture* diffuse, AssetTexture* specular);
    void setPosition(glm::vec3 position);
    /**
     * @param rotationX rotation around x axis in degrees
     * @param rotationY rotation around y axis in degrees
     * @param rotationZ rotation around z axis in degrees
     */
    void setRotation(float rotationX, float rotationY, float rotationZ);
    /**
     * @param angle angle of rotation in degrees
     * @param axis axis of rotation
     */
    void setRotation(float angle, glm::vec3 axis);
    
    virtual void draw(Camera& camera, Shader& shader);
    void drawToDepthMap(PointLightCamera& camera, Shader& depthShader);
protected:
    float rotationX, rotationY, rotationZ;
    glm::mat4 model = glm::mat4(1.0f); // shader uses model to place vertices around correct location in the world
    Mesh mesh;
 
    /**
     * call this after updating rotation or any subclass fields used by generateVertices/Indices
     */
    void invalidateShape();
    /**
     * call this after updating position and/or rotation.
     */
    void invalidateModel();
private:
    glm::vec3 position;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    const std::string TAG = "Shape3D";

    virtual std::vector<Vertex> generateVertices() = 0;
    virtual std::vector<GLuint> generateIndices() = 0;
};

#endif