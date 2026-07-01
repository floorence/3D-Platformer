#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>

enum class ShaderType {
	Vertex, Fragment, Program
};

class Shader {
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	~Shader();

	Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

	void activate();
private:
	const std::string TAG = "Shader";
	void logCompileErrors(unsigned int shader, ShaderType type);
	std::string typeToString(ShaderType type);
	std::string getFileContents(const char* filename);
};

#endif
