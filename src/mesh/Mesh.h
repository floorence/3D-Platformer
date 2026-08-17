#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include"VAO.h"
#include"EBO.h"
#include"camera/Camera.h"
#include "camera/PointLightCamera.h"
#include "shader/Shader.h"

struct Material {
	glm::vec3 color = glm::vec3(1.0f);
	std::vector<Texture*> textures;
};

class Mesh {
public:
	Mesh() = default;
	Mesh(const std::vector<Texture*>& textures);
	Mesh(const std::vector<Vertex>& vertices, const std::vector <GLuint>& indices, const std::vector<Texture*>& textures);
	Mesh(const glm::vec3 color);
	Mesh(const std::vector<Vertex>& vertices, const std::vector <GLuint>& indices, const glm::vec3 color);

	glm::vec3 getColor();
	void setColor(const glm::vec3 color);
	void setTextures(const std::vector<Texture*>& textures);
	void setShapeData(const std::vector<Vertex>& vertices, const std::vector <GLuint>& indices);

	void draw(Camera& camera, Shader& shader);
	void drawGui(Shader& shader); // version of draw that ignores camera
	void drawLine(Camera& camera, Shader& shader); // version of draw that uses GL_LINES
	void drawToDepthMap(PointLightCamera& camera, Shader& depthShader); // version of draw that uses depthShader and ignores colors and textures 
private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	int drawCount = 0;

	ColorSource colorSource = ColorSource::Texture;
	Material material;

	const std::string TAG = "Mesh";

	void prepareColorsAndTextures(Shader& shader);
};

#endif