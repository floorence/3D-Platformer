#include"Quad.h"
#include"mesh/VBO.h"
#include"mesh/Mesh.h"

Quad::Quad(Texture* texture) {
    setTexture(texture);
}

void Quad::setTexture(Texture* texture) {
    mesh.setTextures({texture});
}

void Quad::setTextures(std::vector<Texture*> textures) {
    mesh.setTextures(textures);
}

void Quad::setBounds(float x, float y, float w, float h, bool reversedYAxis) {
    float bottom = reversedYAxis ? y + h : y - h;
    setBounds(x, y, x + w, bottom);
}

void Quad::setBounds(float xu, float yu, float xv, float yv) {
    this->xu = xu;
    this->yu = yu;
    this->xv = xv;
    this->yv = yv;

    std::vector<Vertex> vertices = {
        Vertex {glm::vec3(xu, yu, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex {glm::vec3(xv, yu, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex {glm::vec3(xu, yv, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex {glm::vec3(xv, yv, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)}
    };
    std::vector<GLuint> indices = {0, 2, 3, 0, 3, 1};

    mesh.setShapeData(vertices, indices);
}

void Quad::draw(Shader& shader) {
    if (useColorInsteadOfTexture) {
        shader.setColor(color);
        shader.setColorOverride(true);
    }
    mesh.drawGui(shader);
}