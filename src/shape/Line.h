#ifndef LINE_H
#define LINE_H

#include "shape/Shape3D.h"

class Line: public Shape3D {
public:
    glm::vec3 color = glm::vec3(0.0f);

    Line() = default;
    Line(glm::vec3 start, glm::vec3 end);

    void setCoordinates(glm::vec3 start, glm::vec3 end);
    void draw(Camera& camera, Shader& shader) override;
private:
    glm::vec3 end; // relative to the start of the line (position)!

    std::vector<Vertex> generateVertices() override;
    std::vector<GLuint> generateIndices() override;
};

#endif