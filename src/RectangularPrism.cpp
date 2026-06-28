#include "RectangularPrism.h"
#include <glm/geometric.hpp>
#include <glm/gtc/constants.hpp>

RectangularPrism::RectangularPrism(Texture* diffuse, Texture* specular, glm::vec3 position, float width, float height, float length) 
    : Shape3D(diffuse, specular, position),
      width(width), height(height), length(length)
{
    invalidateShape();
}

std::vector<Vertex> RectangularPrism::generateVertices() {
    std::vector<Vertex> vertices;

    float x = -width / 2.0f;
    float y = -height / 2.0f;
    float z = -length / 2.0f;

    for (int i = 0; i < 8; i++) {
        if (i % 4 == 0) x = -x;
        if (i % 2 == 0) y = -y;
        z = -z;

        // TODO: normals have to be rotated
        glm::vec3 xNormal((x < 0) ? -1.0f : 1.0f, 0.0f, 0.0f);
        glm::vec3 yNormal(0.0f, (y < 0) ? -1.0f : 1.0f, 0.0f);
        glm::vec3 zNormal(0.0f, 0.0f, (z < 0) ? -1.0f : 1.0f);

        glm::vec3 pos(x, y, z);

        // Log::log(TAG, fmt::format("pos = {}, {}, {}", x, y, z));

        glm::vec2 xFacingTexCoord = deriveTexCoord(pos, (x < 0) ? Facing::negX : Facing::posX);
        glm::vec2 yFacingTexCoord = deriveTexCoord(pos, (y < 0) ? Facing::negY : Facing::posY);
        glm::vec2 zFacingTexCoord = deriveTexCoord(pos, (z < 0) ? Facing::negZ : Facing::posZ);

        Vertex xFacing = Vertex{
            pos,
            xNormal,
            glm::vec3(0.0f),
            xFacingTexCoord
        };

        Vertex yFacing = Vertex{
            glm::vec3(x, y, z),
            yNormal,
            glm::vec3(0.0f),
            yFacingTexCoord
        };

        Vertex zFacing = Vertex{
            glm::vec3(x, y, z),
            zNormal,
            glm::vec3(0.0f),
            zFacingTexCoord
        };

        vertices.insert(vertices.end(), {xFacing, yFacing, zFacing});
    }
    return vertices;
}

std::vector<GLuint> RectangularPrism::generateIndices() {
    return {
        0, 3, 9, 0, 6, 9, // facing posX
        12, 15, 21, 12, 18, 21, // facing negX
        1, 4, 16, 1, 13, 16, // facing posY
        7, 10, 22, 7, 19, 22, // facing negY
        2, 8, 20, 2, 14, 20, // facing posZ
        5, 11, 23, 5, 17, 23 // facing negZ
    };
}

glm::vec2 RectangularPrism::deriveTexCoord(glm::vec3 vertex, Facing dir) {
    const float PI = glm::pi<float>();
    glm::mat4 rotation(1.0f);

    // rotate vertex such that it's "facing" posZ so that i can just drop z coord to get texCoord
    switch (dir) {
        case Facing::negX:
            rotation = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case Facing::posX:
            rotation = glm::rotate(glm::mat4(1.0f), -PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case Facing::negY:
            rotation = glm::rotate(glm::mat4(1.0f), -PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case Facing::posY:
            rotation = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        case Facing::negZ:
            rotation = glm::rotate(glm::mat4(1.0f), PI, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        default: break; // do not have to do anything if already facing posZ
    }

    glm::vec3 rotatedVertex = glm::vec3(rotation * glm::vec4(vertex, 1.0f));

    // since tex coords are 0 to 1
    rotatedVertex.x = (rotatedVertex.x < 0) ? 0 : 1;
    rotatedVertex.y = (rotatedVertex.y < 0) ? 0 : 1;

    return glm::vec2(rotatedVertex);
}