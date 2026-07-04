#include"Player.h"

Player::Player(glm::vec3 position, int width, int height) 
    : camera(position, width, height)
{
    this->position = position;
    camera.setPerspective(45.0f, 0.1f, 100.0f);
	mass = 1.0f;

	lastX = camera.width / 2.0;
	lastY = camera.height / 2.0;
}

void Player::handleKeyInputs(GLFWwindow* window, float deltaTime) {
	if (!focused) return;

	glm::vec3 force = glm::vec3(0.0, 0.0, 0.0); // Newtons

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		force += glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		force += -glm::normalize(glm::cross(orientation, camera.UP));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		force += -glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		force += glm::normalize(glm::cross(orientation, camera.UP));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		force += camera.UP;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		force += -camera.UP;
	}

	if (force != glm::vec3(0.0, 0.0, 0.0)) {
		force = 2.0f * glm::normalize(force);
		applyForce(force);
	}

	updatePosition(deltaTime);
    camera.position = this->position;
    camera.lookAt(orientation);
}

void Player::handleKeyInputs(GLFWwindow* window, int key, int action) {
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

void Player::handleFocusChange(GLFWwindow* window) {
	if (focused) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		focused = false;
	} else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		firstClick = true;
		focused = true;
	}
}

void Player::handleMousePos(double xpos, double ypos) {
//	Log::log(TAG, Log::oss("handleMousePos: ", xpos, ", ", ypos));
	if (!focused) return;

	if (firstClick) {
		lastX = xpos;
		lastY = ypos;
		firstClick = false;
	}

	double xOffset = xpos - lastX;
	double yOffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	float rotX = sensitivity * (float)(xOffset) / camera.width;
	float rotY = sensitivity * (float)(yOffset) / camera.height;

	// calculate upcoming vertical change in the orientation
	glm::vec3 verticalOrientation = glm::rotate(orientation, glm::radians(-rotY), glm::normalize(glm::cross(orientation, camera.UP)));

	// decide whether or not the next vertical orientation is legal or not
	if (std::abs(glm::angle(verticalOrientation, camera.UP) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
		orientation = verticalOrientation;
	}

	// rotate the orientation left and right
	orientation = glm::rotate(orientation, glm::radians(-rotX), camera.UP);
}
