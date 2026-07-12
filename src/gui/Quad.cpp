#include"Quad.h"
#include"mesh/VBO.h"
#include"mesh/Mesh.h"

Quad::Quad(Texture* texture): tex(texture) {}

void Quad::setTexture(Texture* texture) {
    tex = texture;
}

void Quad::draw(Shader& shader, float x, float y, float w, float h, bool reversedYAxis) {
    float bottom = reversedYAxis ? y + h : y - h;
    draw(shader, x, y, x + w, bottom);
}

void Quad::draw(Shader& shader, float xu, float yu, float xv, float yv) {
    std::vector<Vertex> vertices = {
        Vertex {glm::vec3(xu, yu, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex {glm::vec3(xv, yu, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex {glm::vec3(xu, yv, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex {glm::vec3(xv, yv, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)}
    };
    std::vector<GLuint> indices = {0, 2, 3, 0, 3, 1};
    std::vector<Texture*> textures = {tex};
    
    Mesh mesh(vertices, indices, textures);
    mesh.drawGui(shader);
}