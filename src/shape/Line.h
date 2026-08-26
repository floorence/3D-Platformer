#ifndef LINE_H
#define LINE_H

#include "shape/Shape3D.h"

class Line: public Shape3D {
public:
    Line() = default;
    Line(glm::vec3 start, glm::vec3 end);

    void setCoordinates(glm::vec3 start, glm::vec3 end);
    void draw(Camera& camera) override;
private:
    glm::vec3 end; // relative to the start of the line (position)!

    std::vector<Vertex> generateVertices() override;
    std::vector<GLuint> generateIndices() override;
};

#endif