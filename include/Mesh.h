#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

struct Material {
	Shader* shader;
	std::vector <Texture*> textures;
};

class Mesh {
public:
	Mesh(const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices, const Material& material);

	void draw(Camera& camera);
	void drawGui();
private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	int drawCount;
	Material material;
	const std::string TAG = "Mesh";
};

#endif