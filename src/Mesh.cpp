#include "Mesh.h"
#include<string>

Mesh::Mesh(
	std::vector <Vertex>& vertices, 
	std::vector <GLuint>& indices, 
	Material& material
)
	: material(material)
{
	drawCount = indices.size();

	vao.bind();
	vbo.linkVertices(vertices);
	ebo.linkIndices(indices);
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


void Mesh::draw(Camera& camera) {
	// Bind shader to be able to access uniforms
	material.shader->activate();
	vao.bind();

	// Keep track of how many of each type of textures we have
	unsigned int numDiffuse = 0;
	unsigned int numSpecular = 0;

	for (unsigned int i = 0; i < material.textures.size(); i++) {
		std::string num;
		TextureType type = material.textures[i]->type;
		if (type == TextureType::Diffuse) {
			num = std::to_string(numDiffuse++);
		}
		else if (type == TextureType::Specular) {
			num = std::to_string(numSpecular++);
		}
		std::string typeString = material.textures[i]->typeToString(material.textures[i]->type);
		material.textures[i]->setTexUnit(*material.shader, (typeString + num).c_str(), i);
		material.textures[i]->bind(i);
	}
	// Take care of the camera Matrix
	glUniform3f(glGetUniformLocation(material.shader->ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.exportMatrix(*material.shader, "camMatrix");

	// Draw the actual mesh
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
}