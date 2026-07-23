#include"Player.h"
#include"util/Utils.h"

Player::Player(glm::vec3 position, int windowWidth, int windowHeight) 
    : camera(position, windowWidth, windowHeight),
	  thirdPersonCam(position, windowWidth, windowHeight),
	  body(position, 0.1f, 0.1f, 0.1f)
{
    this->position = position;
    camera.setPerspective(45.0f, 0.1f, 100.0f);
    thirdPersonCam.setPerspective(45.0f, 0.1f, 100.0f);
	mass = 1.0f;

	lastX = camera.windowWidth / 2.0;
	lastY = camera.windowHeight / 2.0;

	thirdPersonCam.position = glm::vec3(position.x, position.y, position.z + thirdPersonDist);
	syncCamerasAndBody();
}

void Player::setTextures(AssetTexture* diffuse, AssetTexture* specular) {
	body.setTextures(diffuse, specular);
}

Camera* Player::getActiveCamera() {
	return (thirdPerson) ? &thirdPersonCam : &camera;
}

std::string Player::getDebugString() {
	return fmt::format("position: {:.3f}, {:.3f}, {:.3f}\nthirdPerson: {:.3f}, {:.3f}, {:.3f}",
        position.x, position.y, position.z,
		thirdPersonCam.position.x, thirdPersonCam.position.y, thirdPersonCam.position.z
	);
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
	syncCamerasAndBody();
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
	if (key == GLFW_KEY_P) {
		if (action == GLFW_PRESS) thirdPerson = !thirdPerson;
	}
}

void Player::handleMousePos(GLFWwindow* window, double xpos, double ypos) {
//	Log::log(TAG, Log::oss("handleMousePos: ", xpos, ", ", ypos));
	glm::vec3* activeOrientation = &orientation;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && thirdPerson) {
		activeOrientation = &thirdPersonOrientation;
	}

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

	float rotX = sensitivity * (float)(xOffset) / camera.windowWidth;
	float rotY = sensitivity * (float)(yOffset) / camera.windowHeight;

	// calculate upcoming vertical change in the orientation
	glm::vec3 verticalOrientation = glm::rotate(*activeOrientation, glm::radians(-rotY), glm::normalize(glm::cross(*activeOrientation, Camera::UP)));

	// decide whether or not the next vertical orientation is legal or not
	if (std::abs(glm::angle(verticalOrientation, Camera::UP) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
		*activeOrientation = verticalOrientation;
	}

	// rotate the orientation left and right
	*activeOrientation = glm::rotate(*activeOrientation, glm::radians(-rotX), Camera::UP);
	// lookAt is already called every frame in syncCamerasAndBody()
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

void Player::syncCamerasAndBody() {
	camera.position = this->position;
	camera.lookAt(orientation);

	glm::vec3 playerTo3rdPerson = Utils::setVectorLength(thirdPersonCam.position - camera.position, thirdPersonDist);
	thirdPersonCam.position = camera.position + playerTo3rdPerson;
	thirdPersonCam.lookAt(thirdPersonOrientation);

	// TODO: rotate body based on orientation and when looking around in third person make player always be in the middle
	body.setPosition(camera.position);
}

