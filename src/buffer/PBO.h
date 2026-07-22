#ifndef PBO_H
#define PBO_H

#include<glad/glad.h>

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

#endif