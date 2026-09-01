#pragma once

#include <glad/glad.h>

class PBO {
public:
	GLuint ID;

	PBO();
	~PBO();

	PBO(const PBO&) = delete;
    PBO& operator=(const PBO&) = delete;

    PBO(PBO&& other) noexcept;
    PBO& operator=(PBO&& other) noexcept;

	void bind();
};
