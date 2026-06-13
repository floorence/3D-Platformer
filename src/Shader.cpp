#include"Shader.h"
#include<fstream>
#include<iostream>
#include<cerrno>
#include"Log.h"

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	// Read vertexFile and fragmentFile and store the strings
	std::string vertexCode = getFileContents(vertexFile);
	std::string fragmentCode = getFileContents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	logCompileErrors(vertexShader, ShaderType::Vertex);

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
	logCompileErrors(fragmentShader, ShaderType::Fragment);

	// Create Shader Program Object and get its reference
	ID = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	logCompileErrors(ID, ShaderType::Program);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::activate() {
	glUseProgram(ID);
}

Shader::~Shader() {
	glDeleteProgram(ID);
}

// Logs error message if shader has not compiled properly
void Shader::logCompileErrors(unsigned int shader, ShaderType type) {
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != ShaderType::Program) {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			Log::err(TAG, Log::oss("SHADER_COMPILATION_ERROR for:", typeToString(type), "\n", infoLog));
		}
	} else {
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			Log::err(TAG, Log::oss("SHADER_LINKING_ERROR for:", typeToString(type), "\n", infoLog));
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

// Reads a text file and outputs a string with everything in the text file
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
	Log::log(TAG, Log::oss("failed to read shader file: ", filename));
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
