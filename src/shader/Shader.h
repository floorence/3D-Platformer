#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include "camera/Camera.h"
#include "camera/PointLightCamera.h"
#include "texture/CubeMapTexture.h"
#include "texture/Texture.h"
#include<glm/glm.hpp>
#include<string>

enum class ShaderType {
	Vertex, Geometry, Fragment, Program
};

class Shader {
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(const char* vertexFile, const char* geometryFile, const char* fragmentFile);
	~Shader();

	Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

	void activate();
	void setModel(glm::mat4 model); // default
	void setCamera(Camera& camera); // default
	void setPointLightCamera(PointLightCamera& camera); // depth
	void setFarPlane(float farPlane); // default
	void setTexture(Texture& texture, std::string uniform, GLuint unit); // default, gui, and hdr
	void setCubeMapTexture(CubeMapTexture& texture, std::string uniform, GLuint unit); // default
	void setProjection(glm::mat4 projection); // gui

    void registerLightSource(int num, glm::vec3 lightColor, glm::vec3 lightPos, float linear, float quadratic); // default
    void setNumPointLights(int num); // default

	void setColor(glm::vec3 color); // light
	void setColorTint(glm::vec3 color, float intensity); // default
private:
	const std::string TAG = "Shader";
	GLuint createShader(const char* source, ShaderType type);
	void logCompileErrors(unsigned int shader, ShaderType type);
	std::string typeToString(ShaderType type);
	std::string getFileContents(const char* filename);
};

#endif
