#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include<string>

#include"VAO.h"
#include"EBO.h"
#include"Camera.h"
#include"Texture.h"

class Mesh {
public:
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture*>& textures);

	void draw(Shader& shader, Camera& camera);
private:
	VAO vao;
	VBO vbo;
	EBO ebo;
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture*> textures;
};
#endif