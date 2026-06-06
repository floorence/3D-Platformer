#include "Mesh.h"

Mesh::Mesh(
	std::vector <Vertex>& vertices, 
	std::vector <GLuint>& indices, 
	std::vector <Texture*>& textures
)
	: vbo(vertices),
	  ebo(indices),
	  vertices(vertices),
	  indices(indices),
	  textures(textures)
{
	vao.bind();
	// Links VBO attributes such as coordinates and colors to VAO
	vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}


void Mesh::draw(Shader& shader, Camera& camera) {
	// Bind shader to be able to access uniforms
	shader.activate();
	vao.bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < textures.size(); i++) {
		std::string num;
		TextureType type = textures[i]->type;
		if (type == TextureType::Diffuse)
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == TextureType::Specular)
		{
			num = std::to_string(numSpecular++);
		}
		std::string typeString = textures[i]->typeToString(textures[i]->type);
		textures[i]->setTexUnit(shader, (typeString + num).c_str(), i);
		textures[i]->bind();
	}
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.exportMatrix(shader, "camMatrix");

	ebo.bind();
	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}