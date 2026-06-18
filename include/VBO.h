#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include "Log.h"
#include<glm/glm.hpp>
#include<glad/glad.h>
#include <string>
#include<vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;

	std::string to_string() {
		return Log::oss("Vertex { position: ", 
			position.x, ", ", position.y, ", ", position.z, 
			" texUV: ", 
			texUV.x, ", ", texUV.y, "}");
	}
};

class VBO {
public:
	GLuint ID;

	VBO();
	VBO(std::vector<Vertex>& vertices);
	~VBO();

	VBO(const VBO&) = delete;
    VBO& operator=(const VBO&) = delete;

    VBO(VBO&& other) noexcept;
    VBO& operator=(VBO&& other) noexcept;

	void linkVertices(std::vector<Vertex>& vertices);
	void bind();
	void unbind();
};

#endif
