#include"Trail.h"
#include "util/Globals.h"
#include "util/Log.h"
#include "util/Utils.h"
#include <fmt/format.h>

Trail::Trail(glm::vec3 position, int maxPoints, float headWidth)
    : Shape3D(position, false) 
{
    this->maxPoints = maxPoints;
    this->headWidth = headWidth;
	shader = Globals::FlatShader;
    cullFacesBeforeDraw = false;
}

void Trail::addPoint(glm::vec3 position, float angle) {
    addPointInternal(position - this->position, angle);
}

void Trail::addPointRelative(glm::vec3 position, float angle) {
    addPointInternal(position, angle);
}

void Trail::addPointWithDir(glm::vec3 position, float angle, glm::vec3 direction) {
    addPointInternal(position - this->position, angle, direction);
}

void Trail::addPointInternal(glm::vec3 position, float angle, glm::vec3 direction) {
    // Log::log("Trail", fmt::format("addPoint position: {}, {}, {}, angle: {}", position.x, position.y, position.z, angle));
    glm::vec3 prevHead;
    if (points == 0) {
        // first point ever
        vertices.push_back(Vertex {
            position,
            glm::vec3(0.0f),
            glm::vec2(0.0f),
            glm::vec3(0.0f),
        });
        points++;
        invalidateShape();
        return;
    } else if (points == 1) {
        prevHead = vertices[0].position;
    } else {
        prevHead = (vertices[0].position + vertices[1].position) / 2.0f;
    }
    if (position == prevHead) return;
    if (direction == glm::vec3(0.0f)) direction = position - prevHead;
    glm::vec3 left = glm::cross(Constants::UP, direction);
    left = glm::rotate(left, angle, direction);
    left = Utils::setVectorLength(left, headWidth / 2);
    glm::vec3 up = glm::normalize(glm::cross(direction, left));

    Vertex leftVertex = {position + left, up, glm::vec2(0.0f), glm::vec3(0.0f)};
    Vertex rightVertex = {position - left, up, glm::vec2(0.0f), glm::vec3(0.0f)};
    if (points == 1) vertices[0].normal = up; // update normal of first vertex
    // Log::log("Trail", fmt::format("normal: {}, {}, {}", up.x, up.y, up.z));
    
    std::vector<Vertex> newVertices = {leftVertex, rightVertex};
    vertices.insert(vertices.begin(), newVertices.begin(), newVertices.end());
    points++;
    int removePoint = false;
    if (points > maxPoints) {
        removePoint = true;
        points--;
    }

    // update existing points' widths and remove last point if greater than maxPoints
    for (uint i = 2; i < vertices.size(); i += 2) {
        // Log::log("Trail", fmt::format("i: {} size: {}, points: {}", i, vertices.size(), points));

        if (i / 2 + 1 == points) { // last point
            if (removePoint) {
                // i'm actually the second last point, make myself the last point
                glm::vec3 midPoint = (vertices[i].position + vertices[i + 1].position) / 2.0f;
                vertices[i].position = midPoint;
                vertices.pop_back();
                vertices.pop_back();
            }
            // last point which only has one vertex
            break;
        }
        float width = (points - 1 - i / 2) * (headWidth / (points - 1)); 
        // Log::log("Trail", fmt::format("points: {}, num: {}, width: {}", points, (points - i / 2), width));
        glm::vec3 midPoint = (vertices[i].position + vertices[i + 1].position) / 2.0f;
        glm::vec3 left = vertices[i].position - midPoint;
        left = Utils::setVectorLength(left, width / 2.0f);
        vertices[i].position = midPoint + left;
        vertices[i + 1].position = midPoint - left;
    }

    invalidateShape();
}

std::vector<Vertex> Trail::generateVertices() {
    return vertices;
}

std::vector<GLuint> Trail::generateIndices() {
    if (!indicesMaxPoints) {
        indices.clear();
        for (uint i = 0; i < vertices.size(); i += 2) {
            if (i == vertices.size() - 1) {
                // only one vertex
                indices.push_back(i);
                break;
            }
            if (i == vertices.size() - 3) {
                indices.insert(indices.end(), {i, i + 2, i + 1});
                break;
            }
            indices.insert(indices.end(), {i, i + 2, i + 3, i, i + 3, i + 1});
        }
        indicesMaxPoints = points == maxPoints;
    } 
    return indices;
}
