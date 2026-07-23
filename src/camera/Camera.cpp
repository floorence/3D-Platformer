#include"Camera.h"
#include"util/Log.h"

Camera::Camera(glm::vec3 position, int windowWidth, int windowHeight)
	: position(position), windowWidth(windowWidth), windowHeight(windowHeight) {}

void Camera::setPerspective(float FOVdeg, float nearPlane, float farPlane) {
	// add perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)windowWidth / windowHeight, nearPlane, farPlane);
	this->farPlane = farPlane;
}

void Camera::lookAt(glm::vec3 orientation, glm::vec3 up) {
	// make camera look in the right direction from the right position
	view = glm::lookAt(position, position + orientation, up);
}

glm::mat4 Camera::getCameraMatrix() {
	return projection * view;
}
