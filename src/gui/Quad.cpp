#include"Quad.h"
#include"mesh/VBO.h"
#include"mesh/Mesh.h"

Quad::Quad(Texture* texture): tex(texture) {}

void Quad::draw(Shader& shader, CoordinateSystem2D coordSystem, float x, float y, float w, float h) {
    float bottom;
    // TODO: I actually don't care about the exact coordinate system, i just need to know which direction y increases in
    if (coordSystem == CoordinateSystem2D::CENTER) {
        bottom = y - h;
    } else {
        bottom = y + h;
    }
    std::vector<Vertex> vertices = {
        Vertex {glm::vec3(x, y, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f)},
        Vertex {glm::vec3(x + w, y, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f)},
        Vertex {glm::vec3(x, bottom, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f)},
        Vertex {glm::vec3(x + w, bottom, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f)}
    };
    std::vector<GLuint> indices = {0, 2, 3, 0, 3, 1};
    std::vector<Texture*> textures = {tex};
    
    Mesh mesh(vertices, indices, textures);
    mesh.drawGui(shader);
}