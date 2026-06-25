#include "Mesh.h"
#include<string>
#include"Log.h"

Mesh::Mesh(const Material& material) : material(material) {}

Mesh::Mesh(
	const std::vector <Vertex>& vertices, 
	const std::vector <GLuint>& indices, 
	const Material& material
)
	: material(material)
{
	setShapeData(vertices, indices);
}

void Mesh::setMaterial(const Material& material) {
	this->material = material;
}

void Mesh::setShapeData(const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices) {
	drawCount = indices.size();

	vao.bind();
	vbo.linkVertices(vertices);
	ebo.linkIndices(indices);
	// Links VBO attributes such as coordinates and colors to VAO
	vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));

	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

void Mesh::draw(Camera& camera) {
	if (material.shader == nullptr) {
		Log::err(TAG, "draw() called when shader is null! not drawing anything.");
		return;
	}

	material.shader->activate(); // bind shader to be able to access uniforms
	vao.bind();

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

		material.textures[i]->exportTexture(*material.shader, (typeString + num).c_str(), i);
	}

	camera.exportCamera(*material.shader, "camPos", "camMatrix");

	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
}

void Mesh::drawGui() {
	if (material.shader == nullptr || material.textures.empty()) {
		Log::err(TAG, "drawGui() called when shader is null or textures is empty! not drawing anything.");
		return;
	}

	material.shader->activate(); // bind shader to be able to access uniforms
	vao.bind();
	
	material.textures[0]->exportTexture(*material.shader, "diffuse0", 0);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
}