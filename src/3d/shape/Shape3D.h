#ifndef SHAPE_3D_CLASS
#define SHAPE_3D_CLASS

#include "mesh/Mesh.h"
#include "3d/Object3D.h"
#include "texture/AssetTexture.h"

class Shape3D: public Object3D {
public:
    bool isLightSource = false;

    // if !isLightSource, will be the tint colour of the light. else, will not be used
    // higher alpha value will lead to stronger tint
    glm::vec4 tintColor = glm::vec4(0.0f);  
    glm::vec3 direction = glm::vec3(0.0f); // 0 for point light, not 0 for spot light

    Shape3D() = default;
    Shape3D(glm::vec3 position, bool isLightSource);
    Shape3D(AssetTexture* diffuse, AssetTexture* specular, glm::vec3 position, bool isLightSource);
    virtual ~Shape3D() = default;

    glm::vec3 getColor();
    void setColor(glm::vec3 color);
    void setTextures(AssetTexture* diffuse, AssetTexture* specular);
    
    virtual void draw(Camera& camera) override;
    void drawToDepthMap(PointLightCamera& camera, Shader& depthShader) override;
protected:
    Mesh mesh;
 
    virtual void preDraw() override;
    /**
     * call this after updating rotation or any subclass fields used by generateVertices/Indices
     */
    void invalidateShape();
private:
    const std::string TAG = "Shape3D";

    virtual std::vector<Vertex> generateVertices() = 0;
    virtual std::vector<GLuint> generateIndices() = 0;
};

#endif