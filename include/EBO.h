#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<vector>
#include<iostream>

class EBO {
public:
	GLuint ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	EBO(std::vector<GLuint>& indices);
	~EBO();

	EBO(const EBO&) = delete;
    EBO& operator=(const EBO&) = delete;

    EBO(EBO&& other) noexcept;
    EBO& operator=(EBO&& other) noexcept;

	void bind();
	void unbind();
};

#endif
