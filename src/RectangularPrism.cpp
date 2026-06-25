#include "RectangularPrism.h"
#include <glm/geometric.hpp>

RectangularPrism::RectangularPrism(Texture* diffuse, Texture* specular, glm::vec3 position, int width, int length, int height) 
    : Shape3D(diffuse, specular, position)
{
    mesh.setShapeData(        
        generateVertices(width, length, height),
        generateIndices()
    );
}

std::vector<Vertex> RectangularPrism::generateVertices(int width, int length, int height) {
    std::vector<Vertex> vertices;

    float x = width / 2.0f;
    float y = height / 2.0f;
    float z = length / 2.0f;

    for (int i = 0; i < 8; i++) {
        if (i % 4 == 1) x = -x;
        if (i % 3 == 1) y = -y;
        if (i % 2 == 1) z = -z;

        // TODO: normals have to be rotated
        glm::vec3 xNormal((x < 0) ? -1.0f : 1.0f, 0.0f, 0.0f);
        glm::vec3 yNormal(0.0f, (y < 0) ? -1.0f : 1.0f, 0.0f);
        glm::vec3 zNormal(0.0f, 0.0f, (z < 0) ? -1.0f : 1.0f);

        glm::vec3 pos(x, y, z);

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
    
}

glm::vec2 RectangularPrism::deriveTexCoord(glm::vec3 vertex, Facing dir) {
    const float PI = acos(-1.0f);
    glm::mat4 rotation(1.0f);
    glm::vec3 normalizedVertex = glm::normalize(vertex); // since tex coords are -1 to 1

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
            rotation = glm::rotate(glm::mat4(1.0f), PI / 2, glm::vec3(0.0f, 1.0f, 0.0f));
            break;
        case Facing::negZ:
            rotation = glm::rotate(glm::mat4(1.0f), PI, glm::vec3(1.0f, 0.0f, 0.0f));
            break;
        default: break; // do not have to do anything if already facing posZ
    }

    glm::vec3 rotatedVertex = glm::vec3(rotation * glm::vec4(normalizedVertex, 1.0f));
    return glm::vec2(rotatedVertex);
}