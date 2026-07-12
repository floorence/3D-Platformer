#include "Mesh.h"
#include<string>
#include"util/Log.h"

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

void Mesh::setTextures(const std::vector<Texture*>& textures) {
	this->textures = textures;
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

	vao.unbind();
	vbo.unbind();
	ebo.unbind();
}

void Mesh::draw(Camera& camera, Shader& shader) {
	shader.activate(); // bind shader to be able to access uniforms
	vao.bind();

	for (unsigned int i = 0; i < textures.size(); i++) {
		shader.setTexture(*textures[i], textures[i]->getUniformName(), i);
	}

	shader.setCamera(camera);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
}

void Mesh::drawGui(Shader& shader) {
	if (textures.empty()) {
		Log::err(TAG, "drawGui() called when textures is empty! not drawing anything.");
		return;
	}

	shader.activate(); // bind shader to be able to access uniforms
	vao.bind();
	// TODO: wait, we can just make setTexture take just the texture and call getUniformName on it
	shader.setTexture(*textures[0], textures[0]->getUniformName(), 0);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
}

void Mesh::drawToDepthMap(PointLightCamera& camera, Shader& depthShader) {
	depthShader.activate();
	vao.bind();

	// depth map does not have textures

	depthShader.setPointLightCamera(camera);
	glDrawElements(GL_TRIANGLES, drawCount, GL_UNSIGNED_INT, 0);
}