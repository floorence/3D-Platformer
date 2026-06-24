#include"Sphere.h"

Sphere::Sphere(Texture* diffuse, Texture* specular, glm::vec3 position, int radius) 
    : shader("shader/default.vert", "shader/default.frag"),
      mesh(
        generateVertices(radius, NUM_STACKS, NUM_SECTORS),
        generateIndices(NUM_STACKS, NUM_SECTORS),
        Material {&shader, {diffuse, specular}}
      )
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
    configureShader(model);
}

// adapted from https://www.songho.ca/opengl/gl_sphere.html#example_sphere
//              Sphere::buildVerticesSmooth() (first half)
std::vector<Vertex> Sphere::generateVertices(int radius, int stacks, int sectors) {
    std::vector<Vertex> vertices;
    const float PI = acos(-1.0f);

    float sectorStep = 2 * PI / sectors;
    float stackStep = PI / stacks;

    for (int i = 0; i <= stacks; i++) {
        float stackAngle = PI / 2 - i * stackStep; // pi/2 to -pi/2 (instead of -pi/2 to pi/2 since tex coords start at the top)
        float xz = radius * cosf(stackAngle); // r * cos(u) -> horizontal distance from middle of sphere
        float y = radius * sinf(stackAngle); // r * sin(u) -> vertical distance from middle of sphere

        // add (sectors+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectors; j++) {
            float sectorAngle = j * sectorStep; // 0 to 2pi

            // vertex position
            float x = xz * cosf(sectorAngle); // r * cos(u) * cos(v)  
            float z = xz * sinf(sectorAngle); // r * cos(u) * sin(v)

            // vertex tex coord between [0, 1]
            float u = (float)j / sectors;
            float v = (float)i / stacks;

            Vertex vertex = Vertex {
                glm::vec3(x, y, z),
                glm::vec3(x, y, z), // normals same as position for smooth lighting
                glm::vec3(0.39f, 0.73f, 0.36f),
                glm::vec2(u, v)
            };

            vertices.push_back(vertex);
        }
    }
    return vertices;
}

// adapted from https://www.songho.ca/opengl/gl_sphere.html#example_sphere
//              Sphere::buildVerticesSmooth() (second half)
std::vector<GLuint> Sphere::generateIndices(int stacks, int sectors) {
    std::vector<GLuint> indices;
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    for (int i = 0; i < stacks; i++) {
        unsigned int k1 = i * (sectors + 1); // beginning of current stack
        unsigned int k2 = k1 + sectors + 1; // beginning of next stack

        for (int j = 0; j < sectors; j++, k1++, k2++) {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0) {
                indices.insert(indices.end(), {k1, k2, k1 + 1});
            }

            if (i != (stacks-1)) {
                indices.insert(indices.end(), {k1 + 1, k2, k2 + 1});
            }
        }
    }
    return indices;
}

void Sphere::configureShader(glm::mat4 model) {
	shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Sphere::registerLightSource(glm::vec4 lightColor, glm::vec3 lightPos) {
    shader.activate();
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
}

void Sphere::draw(Camera& camera) {
    mesh.draw(camera);
}