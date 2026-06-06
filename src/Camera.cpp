#include"Camera.h"
#include "GLFW/glfw3.h"

Camera::Camera(int width, int height, glm::vec3 position) {
	Camera::width = width;
	Camera::height = height;
	Camera::position = position;

	lastX = width / 2.0;
	lastY = height / 2.0;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	// Initializes matrices since otherwise they will be the null matrix
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// Makes camera look in the right direction from the right position
	view = glm::lookAt(position, position + orientation, UP);
	// Adds perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::exportMatrix(Shader& shader, const char* uniform) {
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::handleKeyInputs(GLFWwindow* window, float deltaTime) {
	glm::vec3 movement = glm::vec3(0.0, 0.0, 0.0);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		movement += glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		movement += -glm::normalize(glm::cross(orientation, UP));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		movement += -glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		movement += glm::normalize(glm::cross(orientation, UP));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		movement += UP;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		movement += -UP;
	}

	if (movement != glm::vec3(0.0, 0.0, 0.0)) 
		position += speed * deltaTime * glm::normalize(movement);
}

void Camera::handleKeyInputs(GLFWwindow* window, int key, int action) {
	if (key == GLFW_KEY_LEFT_CONTROL) {
		if (action == GLFW_PRESS) {
			speed = 1.5f;
		} else if (action == GLFW_RELEASE) {
			speed = 1.0f;
		}
	}
	if (key == GLFW_KEY_E) {
		if (action == GLFW_PRESS) handleFocusChange(window);
	}
}

void Camera::handleFocusChange(GLFWwindow* window) {
	if (focused) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		focused = false;
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		firstClick = true;
		focused = true;
	}
}

void Camera::handleMousePos(double xpos, double ypos) {
//	Log::log(TAG, Log::oss("handleMousePos: ", xpos, ", ", ypos));
	if (!focused) return;

	if (firstClick) {
		lastX = xpos;
		lastY = ypos;
		firstClick = false;
	}

	// Calculate the difference (delta) since the last frame
	double xOffset = xpos - lastX;
	double yOffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	float rotX = sensitivity * (float)(xOffset) / width;
	float rotY = sensitivity * (float)(yOffset) / height;

	// Calculates upcoming vertical change in the orientation
	glm::vec3 verticalOrientation = glm::rotate(orientation, glm::radians(-rotY), glm::normalize(glm::cross(orientation, UP)));

	// Decides whether or not the next vertical orientation is legal or not
	if (std::abs(glm::angle(verticalOrientation, UP) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
		orientation = verticalOrientation;
	}

	// Rotates the orientation left and right
	orientation = glm::rotate(orientation, glm::radians(-rotX), UP);
}