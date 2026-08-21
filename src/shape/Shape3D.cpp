#include "Shape3D.h"
#include "shape/Drawable3D.h"
#include "util/Log.h"
#include <glm/geometric.hpp>

Shape3D::Shape3D(glm::vec3 position, bool isLightSource) 
    : Object3D(position) 
{
    this->isLightSource = isLightSource;
}

Shape3D::Shape3D(AssetTexture* diffuse, AssetTexture* specular, glm::vec3 position, bool isLightSource) 
    : Shape3D(position, isLightSource)
{
    setTextures(diffuse, specular);
}

void Shape3D::invalidateShape() {
    std::vector<Vertex> vertices = generateVertices();
    std::vector<GLuint> indices = generateIndices();

    mesh.setShapeData(vertices, indices);
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

void Shape3D::draw(Camera& camera, Shader& shader) {
    shader.setModel(model);
    shader.setRotation(rotation); // shader still has to rotate normals
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