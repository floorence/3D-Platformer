#ifndef PLAYER_H
#define PLAYER_H

#include"Mass.h"
#include"Camera.h"

class Player: public Mass {
public:
    Camera camera;

    Player(glm::vec3 position, int width, int height);

	// inputs
	void handleKeyInputs(GLFWwindow* window, float deltaTime);
	void handleKeyInputs(GLFWwindow* window, int key, int action);
	void handleMousePos(double xpos, double ypos);
private:
	const float MAX_SPEED_DEFAULT = 1.0f;
	const float MAX_SPEED_SPRINTING = 2.0f;
	const float ACCELERATION_MULTIPLIER = 1.0f;

	bool firstClick = true;
	bool focused = true;

	double lastX, lastY;

	float maxSpeed = MAX_SPEED_DEFAULT; // units per second
	float sensitivity = 100.0f; 

	void handleFocusChange(GLFWwindow* window);
};

#endif
