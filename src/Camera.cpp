#include"Camera.h"
#include"util/Log.h"

Camera::Camera(glm::vec3 position, int width, int height)
	: position(position), width(width), height(height) {}

void Camera::setPerspective(float FOVdeg, float nearPlane, float farPlane) {
	// add perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);
}

void Camera::lookAt(glm::vec3 orientation) {
	// make camera look in the right direction from the right position
	view = glm::lookAt(position, position + orientation, UP);
}

void Camera::lookAt(glm::vec3 orientation, glm::vec3 up) {
	view = glm::lookAt(position, position + orientation, up);
}

glm::mat4 Camera::getCameraMatrix() {
	return projection * view;
}

void Camera::exportCamera(Shader& shader, const char* posUniform, const char* matUniform) {
	glUniform3f(glGetUniformLocation(shader.ID, posUniform), position.x, position.y, position.z);
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, matUniform), 1, GL_FALSE, glm::value_ptr(getCameraMatrix()));
}
