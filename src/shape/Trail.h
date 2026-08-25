#ifndef TRAIL_H
#define TRAIL_H

#include "mesh/VBO.h"
#include "shape/Shape3D.h"

// 3d trail that is represented by a flat strip shape that is able to turn and rotate and becomes thinner at the tail
// the model position of the trail will be the position offset of the trail, and not necessarily be a position on the trail.
class Trail: public Shape3D {
public:
    /**
     * @param positionOffset position offset of the trail, will not actually be a point on the trail
     * @param maxPoints maximum number of points that the trail will keep track of for its curve
     */
    Trail(glm::vec3 positionOffset, int maxPoints, float headWidth = 0.1f);

    /** 
     * @brief adds a point to the head of the trail at given position.
     *        if maxPoints is reached, also removes last point in the tail.
     * @param position position in world coordinates
     * @param angle (radians) counterclockwise rotation of the head about the direction that the trail is going, which
     *        will be derived as the vector from previous head position to given position.
     */
    void addPoint(glm::vec3 position, float angle);
    /** @brief same as addPoint, but position is relative to the model position of the trail */
    void addPointRelative(glm::vec3 position, float angle);
    /** @brief same as addPoint, but direction is specified and head will be perpendicular to direction */
    void addPointWithDir(glm::vec3 position, float angle, glm::vec3 direction);
private:
    int points = 0;
    int maxPoints = 0;
    bool indicesMaxPoints = false;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    float headWidth = 0.1f;

    std::vector<Vertex> generateVertices() override;
    std::vector<GLuint> generateIndices() override;

    void addPointInternal(glm::vec3 position, float angle, glm::vec3 direction = glm::vec3(0.0f));
};

#endif