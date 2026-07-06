#include"Shader.h"
#include <fmt/format.h>
#include<fstream>
#include <glm/gtc/type_ptr.hpp>
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

void Shader::setModel(glm::mat4 model) {
	activate();
	glUniformMatrix4fv(glGetUniformLocation(ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Shader::setCamera(Camera& camera) {
	activate();
	glUniform3f(glGetUniformLocation(ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	glUniformMatrix4fv(glGetUniformLocation(ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camera.getCameraMatrix()));
}

void Shader::setPointLightCamera(PointLightCamera& camera) {
	activate();
    std::string uniform = "shadowMatrices[0]";

    for (uint i = 0; i < 6; i++) {
        uniform[uniform.size() - 2] = i + '0';
        glUniformMatrix4fv(glGetUniformLocation(ID, uniform.c_str()), 1, GL_FALSE, glm::value_ptr(camera.shadowTransforms[i]));
    }
	setFarPlane(camera.farPlane);
    glUniform3f(glGetUniformLocation(ID, "lightPos"), camera.position.x, camera.position.y, camera.position.z);
}

void Shader::setFarPlane(float farPlane) {
    glUniform1f(glGetUniformLocation(ID, "farPlane"), farPlane);
}

void Shader::setTexture(Texture& texture, const char* uniform, GLuint unit) {
	activate();

	glUniform1i(glGetUniformLocation(ID, uniform), unit);
	glUniform1f(glGetUniformLocation(ID, "material.shininess"), 16);
	texture.bind(unit);
}

void Shader::setCubeMapTexture(CubeMapTexture& texture, const char* uniform, GLuint unit) {
	activate();

	glUniform1i(glGetUniformLocation(ID, uniform), unit);
	texture.bind(unit);
}

void Shader::registerLightSource(int num, glm::vec3 lightColor, glm::vec3 lightPos, float linear, float quadratic) {
    Log::log(TAG, fmt::format("registerLightSource() num = {} linear = {}, quadratic = {}", num, linear, quadratic));
    activate();

    std::string pointLightUniform = "pointLights[0]";
    pointLightUniform[pointLightUniform.size() - 2] = num + '0';
	glUniform3f(glGetUniformLocation(ID, (pointLightUniform + ".color").c_str()), lightColor.x, lightColor.y, lightColor.z);
	glUniform3f(glGetUniformLocation(ID, (pointLightUniform + ".position").c_str()), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(ID, (pointLightUniform + ".constant").c_str()), 1.0);
	glUniform1f(glGetUniformLocation(ID, (pointLightUniform + ".linear").c_str()), linear);
	glUniform1f(glGetUniformLocation(ID, (pointLightUniform + ".quadratic").c_str()), quadratic);
}

void Shader::setNumPointLights(int num) {
    activate();
	glUniform1i(glGetUniformLocation(ID, "numPointLights"), num);
}

void Shader::setColor(glm::vec3 color) {
	glUniform3f(glGetUniformLocation(ID, "color"), color.x, color.y, color.z);
}

void Shader::setColorTint(glm::vec3 color, float intensity) {
	if (intensity < 0 || intensity > 1) Log::warn(TAG, fmt::format("setColorTint() given colour tint intensity {} is not between 0 and 1!", intensity));
	glUniform3f(glGetUniformLocation(ID, "tintColor"), color.x, color.y, color.z);
	glUniform1f(glGetUniformLocation(ID, "tintIntensity"), intensity);
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
