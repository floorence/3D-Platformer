#include"Camera.h"
#include"util/Log.h"

Camera::Camera(glm::vec3 position, int width, int height)
	: position(position), width(width), height(height) {}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	// make camera look in the right direction from the right position
	glm::mat4 view = glm::lookAt(position, position + orientation, UP);
	// add perspective to the scene
	glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::exportCamera(Shader& shader, const char* posUniform, const char* matUniform) {
	glUniform3f(glGetUniformLocation(shader.ID, posUniform), position.x, position.y, position.z);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, matUniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}
