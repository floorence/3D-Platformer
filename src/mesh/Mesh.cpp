#include "Mesh.h"
#include<string>
#include"util/Log.h"
#include "util/Utils.h"

Mesh::Mesh(const std::vector<Texture*>& textures) 
	: Mesh() 
{
	setTextures(textures);
}

Mesh::Mesh(
	const std::vector <Vertex>& vertices, 
	const std::vector <GLuint>& indices, 
	const std::vector<Texture*>& textures
)
	: Mesh(textures)
{
	setShapeData(vertices, indices);
}

Mesh::Mesh(const glm::vec3 color)
	: Mesh() 
{
	setColor(color);
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector <GLuint>& indices, const glm::vec3 color)
	: Mesh(color)
{
	setShapeData(vertices, indices);
}

glm::vec3 Mesh::getColor() {
	return material.color;
}

void Mesh::setColor(const glm::vec3 color) {
	this->material.color = color;
	colorSource = ColorSource::MaterialColor;
}

void Mesh::setTextures(const std::vector<Texture*>& textures) {
	this->material.textures = textures;
	colorSource = ColorSource::Texture;
}

void Mesh::setShapeData(const std::vector <Vertex>& vertices, const std::vector <GLuint>& indices) {
	drawCount = indices.size();

	vao.bind();
	vbo.linkVertices(vertices);
	ebo.linkIndices(indices);
	// link vertex data from vbo to vao
	vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));
	vao.linkAttrib(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));
	vao.linkAttrib(vbo, 3, 3, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float)));

	Utils::unbindVao();
	Utils::unbindVbo();
	Utils::unbindEbo();
}

void Mesh::draw(Camera& camera, Shader& shader) {
	shader.activate(); // bind shader to be able to access uniforms
	vao.bind();

	shader.setColorSource(colorSource);
	if (colorSource == ColorSource::Texture) {
		for (uint i = 0; i < material.textures.size(); i++) {
			shader.setTexture(*material.textures[i], i);
		}
	} else if (colorSource == ColorSource::MaterialColor) {
		shader.setMaterialColor(material.color);
	}

	shader.setCamera(camera);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
}

void Mesh::drawGui(Shader& shader) {
	shader.activate(); // bind shader to be able to access uniforms
	vao.bind();

	shader.setColorSource(colorSource);
	if (colorSource == ColorSource::Texture) {
		for (uint i = 0; i < material.textures.size(); i++) {
			shader.setTexture(*material.textures[i], i);
		}
	} else if (colorSource == ColorSource::MaterialColor) {
		shader.setColor(material.color);
	}

	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
}

// need this function since line uses gui fragment shader which has different uniform name than default shader
void Mesh::drawLine(Camera& camera, Shader& shader) {
	shader.activate(); // bind shader to be able to access uniforms
	vao.bind();

	shader.setColorSource(colorSource);
	shader.setColor(material.color);
	shader.setCamera(camera);
	glDrawElements(GL_LINES, drawCount, GL_UNSIGNED_INT, 0);
}

void Mesh::drawToDepthMap(PointLightCamera& camera, Shader& depthShader) {
	depthShader.activate();
	vao.bind();

	// depth map does not have textures or colours

	depthShader.setPointLightCamera(camera);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
}