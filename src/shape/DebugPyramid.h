#ifndef DEBUG_PYRAMID_H
#define DEBUG_PYRAMID_H

#include "Shape3D.h"

class DebugPyramid: public Shape3D {
public:
    DebugPyramid(Texture* diffuse, Texture* specular, glm::vec3 position) 
        : Shape3D(diffuse, specular, position, false)
    { invalidateShape(); }

private:
    std::vector<Vertex> generateVertices() override {
        return {
            Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},  // Bottom side
            Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 5.0f)},  // Bottom side
            Vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 5.0f)},  // Bottom side
            Vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(5.0f, 0.0f)},  // Bottom side

            Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(0.0f, 0.0f)},  // Left Side
            Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(5.0f, 0.0f)},  // Left Side
            Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(-0.8f, 0.5f, 0.0f), glm::vec2(2.5f, 5.0f)},  // Left Side

            Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(5.0f, 0.0f)},  // Non-facing side
            Vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(0.0f, 0.0f)},  // Non-facing side
            Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(0.0f, 0.5f, -0.8f), glm::vec2(2.5f, 5.0f)},  // Non-facing side

            Vertex{glm::vec3( 0.5f, 0.0f, -0.5f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(0.0f, 0.0f)},  // Right side
            Vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(5.0f, 0.0f)},  // Right side
            Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(0.8f, 0.5f,  0.0f), glm::vec2(2.5f, 5.0f)},  // Right side

            Vertex{glm::vec3( 0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(5.0f, 0.0f)},  // Facing side
            Vertex{glm::vec3(-0.5f, 0.0f,  0.5f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(0.0f, 0.0f)},  // Facing side
            Vertex{glm::vec3( 0.0f, 0.8f,  0.0f), glm::vec3(0.0f, 0.5f,  0.8f), glm::vec2(2.5f, 5.0f)},  // Facing side
        };
    };

    std::vector<GLuint> generateIndices() override {
        return {
            0, 1, 2, // Bottom side
            0, 2, 3, // Bottom side
            4, 6, 5, // Left side
            7, 9, 8, // Non-facing side
            10, 12, 11, // Right side
            13, 15, 14 // Facing side
        };
    };
};

#endif