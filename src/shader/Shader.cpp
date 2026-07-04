#include"Shader.h"
#include <fmt/format.h>
#include<fstream>
#include<iostream>
#include"util/Log.h"

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = getFileContents(vertexFile);
	std::string fragmentCode = getFileContents(fragmentFile);

	GLuint vertexShader = createShader(vertexCode.c_str(), ShaderType::Vertex);
	GLuint fragmentShader = createShader(fragmentCode.c_str(), ShaderType::Fragment);

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

Shader::Shader(const char* vertexFile, const char* geometryFile, const char* fragmentFile) {
	std::string vertexCode = getFileContents(vertexFile);
	std::string geometryCode = getFileContents(geometryFile);
	std::string fragmentCode = getFileContents(fragmentFile);

	GLuint vertexShader = createShader(vertexCode.c_str(), ShaderType::Vertex);
	GLuint geometryShader = createShader(geometryCode.c_str(), ShaderType::Geometry);
	GLuint fragmentShader = createShader(fragmentCode.c_str(), ShaderType::Fragment);

	ID = glCreateProgram();
	// attach the vertex and fragment shaders to the shader program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, geometryShader);
	glAttachShader(ID, fragmentShader);
	// link all the shaders together into the shader program
	glLinkProgram(ID);
	logCompileErrors(ID, ShaderType::Program);

	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
}

void Shader::activate() {
	glUseProgram(ID);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

GLuint Shader::createShader(const char* source, ShaderType type) {
	int glShaderType;
	switch (type) {
		case ShaderType::Vertex: glShaderType = GL_VERTEX_SHADER; break;
		case ShaderType::Geometry: glShaderType = GL_GEOMETRY_SHADER; break;
		case ShaderType::Fragment: glShaderType = GL_FRAGMENT_SHADER; break;
		case ShaderType::Program: 
			Log::err(TAG, "createShader() called with ShaderType Program!");
			return 0;
	}
	GLuint shader = glCreateShader(glShaderType);
	// attach shader code to opengl shader object
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);
	logCompileErrors(shader, type);
	return shader;
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
		case ShaderType::Geometry: return "geometry";
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
