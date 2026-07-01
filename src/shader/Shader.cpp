#include"Shader.h"
#include <fmt/format.h>
#include<fstream>
#include<iostream>
#include"util/Log.h"

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = getFileContents(vertexFile);
	std::string fragmentCode = getFileContents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// attach shader code to opengl shader object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	logCompileErrors(vertexShader, ShaderType::Vertex);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	logCompileErrors(fragmentShader, ShaderType::Fragment);

	ID = glCreateProgram();
	// attach the vertex and fragment shaders to the shader program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// link all the shaders together into the shader program
	glLinkProgram(ID);
	logCompileErrors(ID, ShaderType::Program);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::activate() {
	glUseProgram(ID);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

void Shader::logCompileErrors(unsigned int shader, ShaderType type) {
	GLint hasCompiled;
	char infoLog[1024];

	if (type != ShaderType::Program) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			Log::err(TAG, fmt::format("SHADER_COMPILATION_ERROR for: {}\n{}", typeToString(type), infoLog));
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			Log::err(TAG, fmt::format("SHADER_LINKING_ERROR for: {}\n{}", typeToString(type), infoLog));
		}
	}
}

std::string Shader::typeToString(ShaderType type) {
	switch (type) {
		case ShaderType::Vertex: return "vertex";
		case ShaderType::Fragment: return "fragment";
		case ShaderType::Program: return "program";
	}
	return "";
}

std::string Shader::getFileContents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	Log::err(TAG, fmt::format("failed to read shader file: {}", filename));
	throw(errno);
}

Shader::Shader(Shader&& other) noexcept {
	ID = other.ID;
	other.ID = 0;
}
    
Shader& Shader::operator=(Shader&& other) noexcept {
	if (this != &other) {
		glDeleteProgram(ID);
		ID = other.ID;
		other.ID = 0;
	}
	return *this;
}
