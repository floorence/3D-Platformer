#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<vector>

class EBO {
public:
	GLuint ID;

	EBO();
	EBO(std::vector<GLuint>& indices);
	~EBO();

	EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;

    EBO(EBO&& other) noexcept;
    EBO& operator=(EBO&& other) noexcept;

	void linkIndices(const std::vector<GLuint>& indices);
	void bind();
	void unbind();
};

#endif
