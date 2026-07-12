#include "Shape3D.h"
#include "util/Log.h"

Shape3D::Shape3D(glm::vec3 position, bool isLightSource) 
    : position(position),
      isLightSource(isLightSource) {}

Shape3D::Shape3D(AssetTexture* diffuse, AssetTexture* specular, glm::vec3 position, bool isLightSource) 
    : Shape3D(position, isLightSource)
{
    std::vector<Texture*> textures;
    if (diffuse != nullptr) textures.push_back(diffuse);
    if (specular != nullptr) textures.push_back(specular);
    mesh.setTextures(textures);
    model = glm::translate(model, position);
}

void Shape3D::invalidateShape() {
    vertices = generateVertices();
    indices = generateIndices();

    glm::mat4 rotation(1.0f);

    if (rotationX) rotation = glm::rotate(rotation, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotationY) rotation = glm::rotate(rotation, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotationZ) rotation = glm::rotate(rotation, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

    for (auto& vertex: vertices) {
        vertex.normal = glm::vec3(rotation * glm::vec4(vertex.normal, 1.0f));
    }

    mesh.setShapeData(        
        vertices,
        indices
    );
}

void Shape3D::invalidateModel() {
    model = glm::translate(glm::mat4(1.0f), position); // reset model; apply translation first and rotation second since matrix multiplication in glm works from right to left
     
    if (rotationX) model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotationY) model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotationZ) model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Shape3D::setRotation(float rotationX, float rotationY, float rotationZ) {
    this->rotationX = rotationX;
    this->rotationY = rotationY;
    this->rotationZ = rotationZ;
    
    invalidateShape();
    invalidateModel();
}
    
void Shape3D::setColor(glm::vec3 color, float intensity) {
    if (isLightSource) {
        if (intensity < 0) Log::warn(TAG, fmt::format("setColor() given light range {} is less than 0!", intensity));
    }
    this->color = color;
    this->intensity = intensity;
}

void Shape3D::draw(Camera& camera, Shader& shader) {
    shader.setModel(model);
    if (isLightSource) {
        shader.setColor(color);
    } else {
        shader.setColorTint(color, intensity);
    }
    mesh.draw(camera, shader);
}

void Shape3D::drawToDepthMap(PointLightCamera& camera, Shader& depthShader) {
    depthShader.setModel(model);
    mesh.drawToDepthMap(camera, depthShader);
}