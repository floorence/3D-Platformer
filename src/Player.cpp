#include"Player.h"
#include "util/Log.h"
#include"util/Utils.h"

Player::Player(glm::vec3 position, int windowWidth, int windowHeight) 
    : camera(position, windowWidth, windowHeight),
	  thirdPersonCam(position, windowWidth, windowHeight),
	  body("assets/models/spaceship/spaceship.obj", position),
	  lineShader("shader/default.vert", "shader/gui.frag") // we don't want the line affected by lighting
{
    this->position = position;
    camera.setPerspective(45.0f, 0.1f, 100.0f);
    thirdPersonCam.setPerspective(45.0f, 0.1f, 100.0f);
	mass = 1.0f;

	lastX = camera.windowWidth / 2.0;
	lastY = camera.windowHeight / 2.0;

	body.setScale(1.0f / 600.0f);
	body.setDefaultRotation(0.0f, glm::radians(180.0f), 0.0f);

	orientationLine.setColor(glm::vec3(100.0f, 0.0f, 69.0f));
	orientationLine.specialShader = &lineShader;

	thirdPersonCam.position = glm::vec3(position.x, position.y, position.z + thirdPersonDist);
	syncCamerasAndBody(glm::vec3(0.0f), 0.0f, 0.0f, 0.0f);
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

/** tilt mode: 
 *  when presing a or d it tilts you in the direction you pressed
 *    tilt amount is proportional to how long the button was pressed at a maximum of 45 degrees.
 *  when pressing a or d it turns in the direction you pressed, and keeps turning until no longer being pressed
 * turn mode:
 * tilt mode but ship never tilts.
 */

void Player::handleKeyInputs(GLFWwindow* window, float deltaTime) {
	if (!focused) return;

	glm::vec3 force = glm::vec3(0.0, 0.0, 0.0); // Newtons
	int yawTurn = 0;
	int pitchTurn = 0;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		force += orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		yawTurn++;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		// TODO: restrict backward movement
		force -= orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		yawTurn--;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		pitchTurn--;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		pitchTurn++;
	}

	if (force != glm::vec3(0.0, 0.0, 0.0)) {
		force = 2.0f * glm::normalize(force);
		applyForce(force);
	}
	glm::vec3 movement = updatePosition(deltaTime);
	syncCamerasAndBody(movement, yawTurn, pitchTurn, deltaTime);
}

void Player::handleKeyInputs(GLFWwindow* window, int key, int action) {
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
	glm::vec3 verticalOrientation = glm::normalize(glm::rotate(
		newOrientation,
		glm::radians(-rotY),
		glm::normalize(glm::cross(newOrientation, Camera::UP))
	));

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

void Player::onSettingsChanged(const Settings& settings) {
	sensitivity = settings.controls.sensitivity.value;
}

void Player::draw(Camera& camera, Shader& shader) {
	if (thirdPerson) body.draw(camera, shader);
	orientationLine.draw(camera, shader);
}

void Player::drawToDepthMap(PointLightCamera& camera, Shader& depthShader) {
	if (thirdPerson) body.drawToDepthMap(camera, depthShader);
}

void Player::syncCamerasAndBody(glm::vec3 movement, int yawTurn, int pitchTurn, float deltaTime) {
	camera.position = this->position;
	camera.lookAt(orientation);

	thirdPersonCam.position += movement;
	thirdPersonOrientation = position - thirdPersonCam.position;
	thirdPersonCam.lookAt(thirdPersonOrientation);

	body.setPosition(position);

	float tiltAmount = TILT_TURN_SPEED * deltaTime;
	if (yawTurn == 0) {
		roll = Utils::approach(roll, 0.0f, tiltAmount);
	} else {
		roll = std::clamp(roll + yawTurn * tiltAmount, -TILT_MAX, TILT_MAX);
	}
	if (pitchTurn == 0) {
		pitch = Utils::approach(pitch, 0.0f, tiltAmount);
	} else {
		pitch = std::clamp(pitch + pitchTurn * tiltAmount, -TILT_MAX, TILT_MAX);
	}

	yaw += yawTurn * tiltAmount;
	// pitch += pitchTurn * tiltAmount;
	// Log::log("Player", fmt::format("tiltAmount: {}, turnAmount: {}", tiltAmount, turnAmount));

	orientation = glm::rotate(Camera::FORWARD, yaw, Camera::UP);
	glm::vec3 left = glm::normalize(glm::cross(Camera::UP, orientation));
	orientation = glm::rotate(orientation, pitch, left);
	// glm::vec3 up = glm::normalize(glm::cross(orientation, left));

	body.setRotation(yaw, Camera::UP);
    // use local axes, otherwise rotation will be post multiplied against existing rotation
	body.rotate(pitch, Camera::RIGHT); // instead of left since body has default rotation of 180 degrees
	body.rotate(roll, Camera::FORWARD);

	orientationLine.setCoordinates(position, position + orientation);
}
