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
	body.setDefaultRotation(0.0f, 180.0f, 0.0f);

	orientationLine.setColor(glm::vec3(100.0f, 0.0f, 69.0f));
	orientationLine.specialShader = &lineShader;

	thirdPersonCam.position = glm::vec3(position.x, position.y, position.z + thirdPersonDist);
	syncCamerasAndBody(glm::vec3(0.0f), 0.0f);
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
 *    tilt amount is proportional to the ratio of the speed in that direction over speed in orientation direction.
 *    maximum of tilt 45 degrees when speeds are the same
 *  force applied is constant no matter current speed, except if speed to left or right is going to be larger than
 *    speed in orientation direction, then no force should be applied.
 *  when tilting and going forward, ship should turn in direction of tilt
 *    ship should turn towards the direction it's accelerating in, but not instantly be facing that direction
 *  when not going forward but a or d is pressed, ship should not tilt, turn or move FOR NOW
 *  
 * turn mode:
 * tilt mode but ship never tilts.
 */

void Player::handleKeyInputs(GLFWwindow* window, float deltaTime) {
	if (!focused) return;

	glm::vec3 force = glm::vec3(0.0, 0.0, 0.0); // Newtons

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		force += glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		force += -glm::normalize(glm::cross(orientation, Camera::UP));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		// TODO: restrict backward movement
		force += -glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		force += glm::normalize(glm::cross(orientation, Camera::UP));
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
		// orientation = activeOrientation;
	}

	glm::vec3 movement = updatePosition(deltaTime);
	syncCamerasAndBody(movement, deltaTime);
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

void Player::syncCamerasAndBody(glm::vec3 movement, float deltaTime) {
	camera.position = this->position;
	camera.lookAt(orientation);

	thirdPersonCam.position += movement;
	thirdPersonOrientation = position - thirdPersonCam.position;
	thirdPersonCam.lookAt(thirdPersonOrientation);

	body.setPosition(position);
	
	float tilt = 0.0f;
	float vAlongOrientation = glm::dot(velocity, orientation);
	float vAlongLeft = glm::dot(velocity, -glm::normalize(glm::cross(orientation, Camera::UP)));
	if (vAlongOrientation > 0) {
		tilt = (vAlongLeft / vAlongOrientation) * 45;
	}
	float orientationToTarget = 0.0f;
	if (prevAcceleration.x != 0 || prevAcceleration.y != 0 || prevAcceleration.z != 0) {
		orientationToTarget = glm::orientedAngle(orientation, glm::normalize(prevAcceleration), Camera::UP);
		if (abs(orientationToTarget) < 0.001f) orientationToTarget = 0.0f;
		Log::log("Player", fmt::format("orientationToTarget: {}, acceleration: {}, {}, {}", orientationToTarget, prevAcceleration.x, prevAcceleration.y, prevAcceleration.z));
	}
	float turn;
	float maxTurnAmount = TURN_SPEED * deltaTime;
	if (orientationToTarget > 0) {
		turn = std::min(orientationToTarget, glm::radians(maxTurnAmount));
	} else {
		turn = std::max(orientationToTarget, glm::radians(-maxTurnAmount));
	}
	orientation = glm::rotate(orientation, turn, Camera::UP);
	Log::log("Player", fmt::format("turn: {}, orientation: {}, {}, {}", turn, orientation.x, orientation.y, orientation.z));
	float angle = glm::orientedAngle(Camera::FORWARD, glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z)), Camera::UP);
	float rotationX = orientation.x * tilt;
	float rotationY = orientation.y * tilt + glm::degrees(angle);
	float rotationZ = orientation.z * tilt;
	body.setRotation(rotationX, rotationY, rotationZ);

	orientationLine.setCoordinates(position, position + orientation);
}
