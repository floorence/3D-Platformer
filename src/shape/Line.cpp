#include"Line.h"

Line::Line(glm::vec3 start, glm::vec3 end) 
    : Shape3D(start, false),
      end(end - start)
{
    invalidateShape();
}

void Line::setCoordinates(glm::vec3 start, glm::vec3 end) {
    this->end = end - start;
    setPosition(start);
    // setPosition already called invalidateModel
    invalidateShape();
}

std::vector<Vertex> Line::generateVertices() {
    return {Vertex{glm::vec3(0.0f)}, Vertex{end}};
}

std::vector<GLuint> Line::generateIndices() {
    return {0, 1};
}

void Line::draw(Camera& camera, Shader& shader) {
    shader.setModel(model);
    shader.setColor(color);
    shader.setUseColor(true);
    mesh.drawLine(camera, shader);
}
