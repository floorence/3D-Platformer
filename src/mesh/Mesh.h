#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include"VAO.h"
#include"EBO.h"
#include"camera/Camera.h"
#include "camera/PointLightCamera.h"
#include"texture/Texture.h"

struct Material {
	Shader* shader = nullptr;
	std::vector <Texture*> textures;
};

class Mesh {
public:
	Mesh();
	Mesh(const Material& material);
	Mesh(const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices, const Material& material);

	void setMaterial(const Material& material);
	void setShapeData(const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices);

	void draw(Camera& camera);
	void drawGui(); // version of draw that ignores camera and assumes exactly one texture
	void drawToDepthMap(PointLightCamera& camera); // version of draw that uses depthShader and ignores textures 
private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	int drawCount = 0;
	Material material;
    Shader depthShader;
	const std::string TAG = "Mesh";
};

#endif