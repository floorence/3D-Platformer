#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

enum class ShaderType {
	Vertex, Fragment, Program
};

class Shader {
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);
	~Shader();

	Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

	void activate();
private:
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, ShaderType type);
	std::string typeToString(ShaderType type);
	// Reads a text file and outputs a string with everything in the text file
	std::string getFileContents(const char* filename);
};

#endif
