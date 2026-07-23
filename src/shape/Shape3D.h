#ifndef SHAPE_3D_CLASS
#define SHAPE_3D_CLASS

#include"mesh/Mesh.h"
#include"texture/AssetTexture.h"

class Shape3D {
public:
    glm::vec3 position; // TODO should be readonly, use setPosition to set postion
    bool isLightSource = false;

    glm::vec3 color;
    float intensity; // will be interpreted differently based on isLightSource. see setColor()
    glm::vec3 direction = glm::vec3(0.0f); // 0 for point light, not 0 for spot light TODO

    Shape3D() = default;
    Shape3D(glm::vec3 position, bool isLightSource);
    Shape3D(AssetTexture* diffuse, AssetTexture* specular, glm::vec3 position, bool isLightSource);
    virtual ~Shape3D() = default;

    void setTextures(AssetTexture* diffuse, AssetTexture* specular);
    void setPosition(glm::vec3 position);
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
    void draw(Camera& camera, Shader& shader);
    void drawToDepthMap(PointLightCamera& camera, Shader& depthShader);
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
    glm::mat4 model = glm::mat4(1.0f); // shader uses model to place vertices around correct location in the world

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    Mesh mesh;
    const std::string TAG = "Shape3D";

    virtual std::vector<Vertex> generateVertices() = 0;
    virtual std::vector<GLuint> generateIndices() = 0;
};

#endif