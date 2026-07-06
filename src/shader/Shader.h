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
	void setModel(glm::mat4 model);
	void setCamera(Camera& camera);
	void setPointLightCamera(PointLightCamera& camera);
	void setFarPlane(float farPlane);
	void setTexture(Texture& texture, const char* uniform, GLuint unit);
	void setCubeMapTexture(CubeMapTexture& texture, const char* uniform, GLuint unit);

    void registerLightSource(int num, glm::vec3 lightColor, glm::vec3 lightPos, float linear, float quadratic);
    void setNumPointLights(int num);

	void setColor(glm::vec3 color);
	void setColorTint(glm::vec3 color, float intensity);
private:
	const std::string TAG = "Shader";
	GLuint createShader(const char* source, ShaderType type);
	void logCompileErrors(unsigned int shader, ShaderType type);
	std::string typeToString(ShaderType type);
	std::string getFileContents(const char* filename);
};

#endif
