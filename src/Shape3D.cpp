#include "Shape3D.h"

Shape3D::Shape3D(Texture* diffuse, Texture* specular, glm::vec3 position, float rotationX, float rotationY, float rotationZ) 
    : shader("shader/default.vert", "shader/default.frag"),
      mesh(Material{&shader, {diffuse, specular}})
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);

    // apply translation first and rotation second since matrix multiplication in glm works from right to left 
    if (rotationX) model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
    if (rotationY) model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
    if (rotationZ) model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f));

    configureShader(model);
}

void Shape3D::configureShader(glm::mat4 model) {
	shader.activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Shape3D::registerLightSource(glm::vec4 lightColor, glm::vec3 lightPos) {
    shader.activate();
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
}

void Shape3D::draw(Camera& camera) {
    mesh.draw(camera);
}