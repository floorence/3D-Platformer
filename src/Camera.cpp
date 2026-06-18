#include"Camera.h"
#include"Log.h"

Camera::Camera(int width, int height, glm::vec3 position) {
	Camera::width = width;
	Camera::height = height;
	Camera::position = position;

	lastX = width / 2.0;
	lastY = height / 2.0;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
	// Makes camera look in the right direction from the right position
	glm::mat4 view = glm::lookAt(position, position + orientation, UP);
	// Adds perspective to the scene
	glm::mat4 projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

	cameraMatrix = projection * view;
}

void Camera::exportMatrix(Shader& shader, const char* uniform) {
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::handleKeyInputs(GLFWwindow* window, float deltaTime) {
	glm::vec3 acceleration = glm::vec3(0.0, 0.0, 0.0); // units per second per second

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		acceleration += glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		acceleration += -glm::normalize(glm::cross(orientation, UP));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		acceleration += -glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		acceleration += glm::normalize(glm::cross(orientation, UP));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		acceleration += UP;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		acceleration += -UP;
	}

	applyAcceleration(getAirResistance(), deltaTime);

	if (acceleration != glm::vec3(0.0, 0.0, 0.0)) {
		acceleration = ACCELERATION_MULTIPLIER * glm::normalize(acceleration);
		applyAcceleration(acceleration, deltaTime);
		//Log::log(TAG, Log::oss("new velocity: ", velocity.x, ", ", velocity.y, ", ", velocity.z, ", speed: ", glm::length(velocity)));
	} else {
		processVelocity();
	}
	position += velocity * deltaTime;
}

void Camera::handleKeyInputs(GLFWwindow* window, int key, int action) {
	if (key == GLFW_KEY_LEFT_CONTROL) {
		if (action == GLFW_PRESS) {
			maxSpeed = MAX_SPEED_SPRINTING;
		} else if (action == GLFW_RELEASE) {
			maxSpeed = MAX_SPEED_DEFAULT;
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

void Camera::applyAcceleration(glm::vec3 a, float dt) {
	velocity.x += a.x * dt;
	velocity.y += a.y * dt;
	velocity.z += a.z * dt;
	float len2 = glm::length2(velocity);
	if (len2 > maxSpeed * maxSpeed) {
		Log::log(TAG, "reached max speed!");
		velocity * (maxSpeed / std::sqrt(len2));
	}
}

glm::vec3 Camera::getAirResistance() {
	return -velocity * AIR_RESISTANCE_MULTIPLIER;
}

void Camera::processVelocity() {
	if (glm::length(velocity) < STOPPING_SPEED) {
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}