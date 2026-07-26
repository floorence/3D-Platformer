#include"Player.h"
#include"util/Utils.h"

Player::Player(glm::vec3 position, int windowWidth, int windowHeight) 
    : camera(position, windowWidth, windowHeight),
	  thirdPersonCam(position, windowWidth, windowHeight),
	  body(position, 0.1f, 0.1f, 0.1f),
	  lineShader("shader/default.vert", "shader/gui.frag") // we don't want the line affected by lighting
{
    this->position = position;
    camera.setPerspective(45.0f, 0.1f, 100.0f);
    thirdPersonCam.setPerspective(45.0f, 0.1f, 100.0f);
	mass = 1.0f;

	lastX = camera.windowWidth / 2.0;
	lastY = camera.windowHeight / 2.0;

	orientationLine.color = glm::vec3(100.0f, 0.0f, 69.0f);
	orientationLine.specialShader = &lineShader;

	thirdPersonCam.position = glm::vec3(position.x, position.y, position.z + thirdPersonDist);
	syncCamerasAndBody(glm::vec3(0.0f));
}

void Player::setTextures(AssetTexture* diffuse, AssetTexture* specular) {
	body.setTextures(diffuse, specular);
}

std::vector<Shape3D*> Player::getShapes() {
	return {&body, &orientationLine};
}

Camera* Player::getActiveCamera() {
	return (thirdPerson) ? &thirdPersonCam : &camera;
}

glm::vec3* Player::getActiveOrientation() {
	return (thirdPerson) ? &thirdPersonOrientation : &orientation;
}

std::string Player::getDebugString() {
	return fmt::format("position: {:.3f}, {:.3f}, {:.3f}\nthirdPerson: {:.3f}, {:.3f}, {:.3f}",
        position.x, position.y, position.z,
		thirdPersonCam.position.x, thirdPersonCam.position.y, thirdPersonCam.position.z
	);
}

void Player::handleKeyInputs(GLFWwindow* window, float deltaTime) {
	if (!focused) return;

	glm::vec3* activeOrientation = &orientation;
	if (thirdPerson) {
		activeOrientation = &thirdPersonOrientation;
	}

	glm::vec3 force = glm::vec3(0.0, 0.0, 0.0); // Newtons

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		force += glm::normalize(glm::vec3(activeOrientation->x, 0.0f, activeOrientation->z));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		force += -glm::normalize(glm::cross(*activeOrientation, Camera::UP));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		force += -glm::normalize(glm::vec3(activeOrientation->x, 0.0f, activeOrientation->z));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		force += glm::normalize(glm::cross(*activeOrientation, Camera::UP));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		force += Camera::UP;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		force += -Camera::UP;
	}

	if (force != glm::vec3(0.0, 0.0, 0.0)) {
		force = 2.0f * glm::normalize(force);
		applyForce(force);
		orientation = *activeOrientation;
	}

	glm::vec3 movement = updatePosition(deltaTime);
	syncCamerasAndBody(movement);
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

void Player::handleMousePos(GLFWwindow*, double xpos, double ypos) {
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
	glm::vec3 newOrientation = *getActiveOrientation();
	glm::vec3 verticalOrientation = glm::rotate(newOrientation, glm::radians(-rotY), glm::normalize(glm::cross(newOrientation, Camera::UP)));

	// decide whether or not the next vertical orientation is legal or not
	if (std::abs(glm::angle(verticalOrientation, Camera::UP) - glm::radians(90.0f)) <= glm::radians(85.0f)) {
		newOrientation = verticalOrientation;
	}

	// rotate the orientation left and right
	newOrientation = glm::rotate(newOrientation, glm::radians(-rotX), Camera::UP);
	
	if (thirdPerson) {
		thirdPersonCam.position = position - Utils::setVectorLength(newOrientation, thirdPersonDist);
	} else {
		orientation = newOrientation;
	}

	// lookAt is already called every frame in syncCamerasAndBody()
}

void Player::handleMouseScroll(GLFWwindow*, double, double yoffset) {
	// scroll down = zoom in. yoffset is positive when scroll down i think
 
	if (focused && thirdPerson) {
		thirdPersonDist = std::max(thirdPersonDist - (float)yoffset, MIN_THIRD_PERSON_DIST);
		thirdPersonCam.position = position - Utils::setVectorLength(thirdPersonOrientation, thirdPersonDist);
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

void Player::syncCamerasAndBody(glm::vec3 movement) {
	camera.position = this->position;
	camera.lookAt(orientation);

	thirdPersonCam.position += movement;
	thirdPersonOrientation = position - thirdPersonCam.position;
	thirdPersonCam.lookAt(thirdPersonOrientation);

	float angle = glm::angle(Camera::FORWARD, glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z)));
	if (orientation.x > 0.0f) angle = -angle;
	body.setPosition(position);
	body.setRotation(glm::degrees(angle), Camera::UP);

	orientationLine.setCoordinates(position, position + orientation);
}
