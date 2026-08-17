#include "Shape3D.h"
#include "util/Log.h"
#include <glm/geometric.hpp>

Shape3D::Shape3D(glm::vec3 position, bool isLightSource) 
    : isLightSource(isLightSource),
      position(position) {}

Shape3D::Shape3D(AssetTexture* diffuse, AssetTexture* specular, glm::vec3 position, bool isLightSource) 
    : Shape3D(position, isLightSource)
{
    setTextures(diffuse, specular);
    model = glm::translate(model, position);
}

void Shape3D::invalidateShape() {
    vertices = generateVertices();
    indices = generateIndices();

    glm::mat4 rotation(1.0f);

    if (rotationX) rotation = glm::rotate(rotation, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotationY) rotation = glm::rotate(rotation, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotationZ) rotation = glm::rotate(rotation, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

    if (rotationX || rotationY || rotationZ) {
        for (auto& vertex: vertices) {
            vertex.normal = glm::vec3(rotation * glm::vec4(vertex.normal, 1.0f));
        }
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

glm::vec3 Shape3D::getPosition() {
    return position;
}

glm::vec3 Shape3D::getColor() {
    return mesh.getColor();
}

void Shape3D::setColor(glm::vec3 color) {
    mesh.setColor(color);
}

void Shape3D::setTextures(AssetTexture* diffuse, AssetTexture* specular) {
    std::vector<Texture*> textures;
    if (diffuse != nullptr) textures.push_back(diffuse);
    if (specular != nullptr) textures.push_back(specular);
    mesh.setTextures(textures);
}

void Shape3D::setPosition(glm::vec3 position) {
    this->position = position;
    invalidateModel();
}

void Shape3D::setRotation(float rotationX, float rotationY, float rotationZ) {
    this->rotationX = rotationX;
    this->rotationY = rotationY;
    this->rotationZ = rotationZ;
    
    invalidateShape();
    invalidateModel();
}

void Shape3D::setRotation(float angle, glm::vec3 axis) {
    axis = glm::normalize(axis);
    setRotation(angle * axis.x, angle * axis.y, angle * axis.z);
}

void Shape3D::draw(Camera& camera, Shader& shader) {
    shader.setModel(model);
    if (!isLightSource) {
		shader.setShininess(16); // TODO
        shader.setColorTint(tintColor);
    }
    mesh.draw(camera, shader);
}

void Shape3D::drawToDepthMap(PointLightCamera& camera, Shader& depthShader) {
    depthShader.setModel(model);
    mesh.drawToDepthMap(camera, depthShader);
}