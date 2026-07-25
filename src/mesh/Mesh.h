#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include"VAO.h"
#include"EBO.h"
#include"camera/Camera.h"
#include "camera/PointLightCamera.h"
#include "shader/Shader.h"

class Mesh {
public:
	Mesh() = default;
	Mesh(const std::vector<Texture*>& textures);
	Mesh(const std::vector<Vertex>& vertices, const std::vector <GLuint>& indices, const std::vector<Texture*>& textures);

	void setTextures(const std::vector<Texture*>& textures);
	void setShapeData(const std::vector<Vertex>& vertices, const std::vector <GLuint>& indices);

	void draw(Camera& camera, Shader& shader);
	void drawGui(Shader& shader); // version of draw that ignores camera and assumes exactly one texture
	void drawLine(Camera& camera, Shader& shader); // version of draw that ignores textures and uses GL_LINES
	void drawToDepthMap(PointLightCamera& camera, Shader& depthShader); // version of draw that uses depthShader and ignores textures 
private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	int drawCount = 0;
	std::vector<Texture*> textures;
	const std::string TAG = "Mesh";
};

#endif