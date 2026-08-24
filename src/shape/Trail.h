#ifndef TRAIL_H
#define TRAIL_H

#include "mesh/VBO.h"
#include "shape/Shape3D.h"

// 3d trail that is represented by a flat strip shape that is able to turn and rotate and becomes thinner at the tail
class Trail: public Shape3D {
public:
    /**
     * @param position position of the head of the trail
     * @param maxPoints maximum number of points that the trail will keep track of for its curve
     */
    Trail(glm::vec3 position, int maxPoints);

    /** 
     * @brief adds a point to the head of the trail at given position.
     *        if maxPoints is reached, also removes last point in the tail.
     * @param angle (radians) counterclockwise rotation of the head about the direction that the trail is going, which
     *        will be derived as the vector from previous head position to given position.
     */
    void addPoint(glm::vec3 position, float angle);
private:
    int points = 0;
    int maxPoints = 0;
    int indicesNumVertices = 0;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    float headWidth = 0.1f;

    std::vector<Vertex> generateVertices() override;
    std::vector<GLuint> generateIndices() override;
};

#endif